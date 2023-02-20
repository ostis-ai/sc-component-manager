/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/CommonUtils.hpp>

#include "src/manager/commands/sc_component_manager_command.hpp"
#include "src/manager/commands/command_init/repos_downloader/repos_downloader.hpp"
#include "src/manager/commands/command_init/repos_downloader/repos_downloader_handler.hpp"

class ScComponentManagerCommandInit : public ScComponentManagerCommand
{
public:
  explicit ScComponentManagerCommandInit(std::string specificationsPath)
    : m_specificationsPath(std::move(specificationsPath))
  {
  }

  ExecutionResult Execute(ScMemoryContext * context, CommandParameters const & commandParameters) override
  {
    std::pair<std::set<std::string>, std::set<std::string>> repositoryItems;
    std::set<ScAddr, ScAddLessFunc> processedRepositories;
    ProcessRepositories(context, m_specificationsPath, repositoryItems, processedRepositories);

    ExecutionResult executionResult;
    executionResult.insert(executionResult.cbegin(), repositoryItems.second.cbegin(), repositoryItems.second.cend());

    return executionResult;
  }

  void ProcessRepositories(
      ScMemoryContext * context,
      std::string & specificationsPath,
      std::pair<std::set<std::string>, std::set<std::string>> & repositoryItems,
      std::set<ScAddr, ScAddLessFunc> & processedRepositories)
  {
    ReposDownloaderHandler downloaderHandler;
    std::set<std::string> repositoriesLinks;
    std::set<std::string> componentsLinks;

    std::set<ScAddr, ScAddLessFunc> availableRepositories;

    ScAddrVector elementList;
    ScIterator3Ptr iterator3 = context->Iterator3(
        keynodes::ScComponentManagerKeynodes::concept_repository, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (iterator3->Next())
    {
      availableRepositories.insert(iterator3->Get(2));
    }

    for (ScAddr const & repository : availableRepositories)
    {
      repositoriesLinks =
          GetRepositoryAddresses(context, repository, keynodes::ScComponentManagerKeynodes::rrel_repositories);
      componentsLinks =
          GetRepositoryAddresses(context, repository, keynodes::ScComponentManagerKeynodes::rrel_components);

      repositoryItems.first.insert(repositoriesLinks.cbegin(), repositoriesLinks.cend());
      repositoryItems.second.insert(componentsLinks.cbegin(), componentsLinks.cend());
    }

    //    TODO(MksmOrlov): implement recursion to handle repositories
    for (std::string const & repositoryLink : repositoriesLinks)
    {
      downloaderHandler.HandleRepositories(context, repositoryLink, specificationsPath);

      ProcessRepositories(context, specificationsPath, repositoryItems, processedRepositories);
    }

    for (std::string const & componentLink : componentsLinks)
    {
      downloaderHandler.HandleComponents(context, componentLink, specificationsPath);
    }
  }

  static std::set<std::string> GetRepositoryAddresses(
      ScMemoryContext * context,
      ScAddr repository,
      ScAddr attributeRelation)
  {
    std::set<std::string> repositoryAddresses;
    ScIterator5Ptr repositoriesSetIterator = context->Iterator5(
        repository,
        ScType::EdgeAccessConstPosPerm,
        ScType::NodeConst,
        ScType::EdgeAccessConstPosPerm,
        attributeRelation);

    if (repositoriesSetIterator->Next())
    {
      ScAddr repositoryItemsSet = repositoriesSetIterator->Get(2);
      ScIterator3Ptr innerRepositoryItemsIterator =
          context->Iterator3(repositoryItemsSet, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);

      while (innerRepositoryItemsIterator->Next())
      {
        ScAddr innerRepositoryItem = innerRepositoryItemsIterator->Get(2);
        ScAddr repositoryAddress = utils::IteratorUtils::getAnyByOutRelation(
            context, innerRepositoryItem, keynodes::ScComponentManagerKeynodes::rrel_address);

        if (repositoryAddress.IsValid())
        {
          std::string repositoryLink = utils::CommonUtils::getLinkContent(context, repositoryAddress);
          repositoryAddresses.insert(repositoryLink);
        }
      }
    }

    return repositoryAddresses;
  }

protected:
  std::string m_specificationsPath;
};
