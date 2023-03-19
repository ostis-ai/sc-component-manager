/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/CommonUtils.hpp>

#include "src/manager/commands/sc_component_manager_command.hpp"
#include "src/manager/commands/command_init/repos_downloader/repos_downloader.hpp"
#include "src/manager/commands/command_init/repos_downloader/repos_downloader_handler.hpp"
#include "src/manager/commands/command_init/sc_component_manager_command_init.hpp"

ExecutionResult ScComponentManagerCommandInit::Execute(
        ScMemoryContext * context,
        CommandParameters const & commandParameters)
{
    std::pair<std::set<std::string>, std::set<std::string>> repositoryItems;
    ScAddrVector processedRepositories;
    ProcessRepositories(context, m_specificationsPath, repositoryItems, processedRepositories);

    ExecutionResult executionResult;
    executionResult.insert(executionResult.cbegin(), repositoryItems.second.cbegin(), repositoryItems.second.cend());

    return executionResult;
}

void ScComponentManagerCommandInit::ProcessRepositories(
        ScMemoryContext * context,
        std::string & specificationsPath,
        std::pair<std::set<std::string>, std::set<std::string>> & repositoryItems,
        ScAddrVector & processedRepositories)
{
    ReposDownloaderHandler downloaderHandler;
    std::set<std::string> repositoriesLinks;
    std::set<std::string> componentsLinks;

    ScAddrVector availableRepositories = utils::IteratorUtils::getAllWithType(
            context, keynodes::ScComponentManagerKeynodes::concept_repository, ScType::NodeConst);

    //    TODO(MksmOrlov): implement recursion break condition
    //    if (availableRepositories == processedRepositories)
    //    {
    //      SC_LOG_WARNING("return;");
    //      return;
    //    }

    for (ScAddr const & repository : availableRepositories)
    {
        repositoriesLinks =
                GetRepositoryAddresses(context, repository, keynodes::ScComponentManagerKeynodes::rrel_repositories);
        componentsLinks =
                GetRepositoryAddresses(context, repository, keynodes::ScComponentManagerKeynodes::rrel_components);

        repositoryItems.first.insert(repositoriesLinks.cbegin(), repositoriesLinks.cend());
        repositoryItems.second.insert(componentsLinks.cbegin(), componentsLinks.cend());

        for (std::string const & componentLink : componentsLinks)
        {
            downloaderHandler.HandleComponents(context, componentLink, specificationsPath);
        }
    }

    //    TODO(MksmOrlov): implement recursion to handle repositories
    //    for (std::string const & repositoryLink : repositoriesLinks)
    //    {
    //      downloaderHandler.HandleRepositories(repositoryLink, specificationsPath);
    //
    //      ProcessRepositories(context, specificationsPath, repositoryItems, processedRepositories);
    //    }
}

std::set<std::string> ScComponentManagerCommandInit::GetRepositoryAddresses(
        ScMemoryContext * context,
        ScAddr repository,
        ScAddr attributeRelation)
{
    std::set<std::string> repositoryAddresses;
    ScIterator5Ptr repositoriesSetIterator = context->Iterator5(
            repository, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, attributeRelation);

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