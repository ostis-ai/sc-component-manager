/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "src/manager/commands/sc_component_manager_command.hpp"
#include "src/manager/commands/command_init/repos_parser/repos_parser.hpp"
#include "src/manager/commands/command_init/repos_downloader/repos_downloader.hpp"
#include "src/manager/commands/command_init/repos_downloader/repos_downloader_handler.hpp"

class ScComponentManagerCommandInit : public ScComponentManagerCommand
{
public:
  ScComponentManagerCommandInit(std::string reposPath, std::string specificationsPath)
    : m_reposPath(std::move(reposPath))
    , m_specificationsPath(std::move(specificationsPath))
  {
  }

  ExecutionResult Execute(ScMemoryContext * context, CommandParameters const & commandParameters) override
  {
    ExecutionResult executionResult;

    ProcessRepositories(context, m_reposPath, m_specificationsPath);

    return executionResult;
  }

  void ProcessRepositories(ScMemoryContext * context, std::string & reposPath, std::string & specificationsPath)
  {
    ReposParser parser;
    parser.Parse(reposPath);

    // TODO: set system identifier to repository and component nodes
    // TODO: if repository exists, continue
    ScAddr repositoryAddr = context->CreateNode(ScType::NodeConst);
    context->CreateEdge(
        ScType::EdgeAccessConstPosPerm, keynodes::ScComponentManagerKeynodes::concept_repository, repositoryAddr);

    ScAddr componentsSetAddr = context->CreateNode(ScType::NodeConst);
    ScAddr rrelComponentsEdgeAddr =
        context->CreateEdge(ScType::EdgeAccessConstPosPerm, repositoryAddr, componentsSetAddr);
    context->CreateEdge(
        ScType::EdgeAccessConstPosPerm, keynodes::ScComponentManagerKeynodes::rrel_components, rrelComponentsEdgeAddr);

    ScAddr repositoriesSetAddr = context->CreateNode(ScType::NodeConst);
    ScAddr rrelRepositoriesEdgeAddr =
        context->CreateEdge(ScType::EdgeAccessConstPosPerm, repositoryAddr, repositoriesSetAddr);
    context->CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        keynodes::ScComponentManagerKeynodes::rrel_repositories,
        rrelRepositoriesEdgeAddr);

    ReposDownloaderHandler downloaderHandler;
    std::vector<std::string> const parsedComponents = parser.GetComponents();
    for (std::string const & componentPath : parsedComponents)
    {
      downloaderHandler.HandleComponents(context, componentPath, specificationsPath, componentsSetAddr);
    }

    std::vector<std::string> const parsedRepositories = parser.GetRepositories();
    for (std::string const & repositoryPath : parsedRepositories)
    {
      downloaderHandler.HandleRepositories(repositoryPath, specificationsPath);
      std::stringstream reposPathStream;
      reposPathStream << specificationsPath << "/" << SpecificationConstants::REPOS_FILENAME;
      reposPath = reposPathStream.str();
      ProcessRepositories(context, reposPath, specificationsPath);
    }
  }

protected:
  std::string m_reposPath;

  std::string m_specificationsPath;
};
