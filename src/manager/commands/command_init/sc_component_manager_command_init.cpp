/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/CommonUtils.hpp>

#include "src/manager/commands/sc_component_manager_command.hpp"
#include "src/manager/commands/command_init/repos_downloader/downloader_handler.hpp"
#include "src/manager/commands/command_init/sc_component_manager_command_init.hpp"

ExecutionResult ScComponentManagerCommandInit::Execute(
    ScMemoryContext * context,
    CommandParameters const & commandParameters)
{
  std::pair<std::set<std::string>, std::set<std::string>> repositoryItems;
  ScAddrVector processedRepositories;

  ScAddrVector availableRepositories = utils::IteratorUtils::getAllWithType(
      context, keynodes::ScComponentManagerKeynodes::concept_repository, ScType::NodeConst);

  ProcessRepositories(context, availableRepositories);

  ExecutionResult executionResult;
  executionResult.insert(executionResult.cbegin(), repositoryItems.second.cbegin(), repositoryItems.second.cend());

  return executionResult;
}

/**
 * @brief Recursivly iterates through repositories
 * and download avaible components specifications.
 * @param context current sc-memory context
 * @param avaibleRepositories vector of avaible repositories addrs
 */
void ScComponentManagerCommandInit::ProcessRepositories(ScMemoryContext * context, ScAddrVector & availableRepositories)
{
  if (availableRepositories.empty())
    return;

  // Get last avaible repository from vector
  ScAddr const & repository = availableRepositories.back();

  ScAddrVector currentRepositoriesAddrs;

  try
  {
    // Get all avaible repositories for current repository
    currentRepositoriesAddrs = GetSpecificationsAddrs(
        context, repository, keynodes::ScComponentManagerKeynodes::rrel_repositories_specifications);
  }
  catch (utils::ScException const & exception)
  {
    SC_LOG_ERROR(exception.Message());
  }

  // Add found repositories to avaible repositories
  availableRepositories.insert(
      availableRepositories.end(), currentRepositoriesAddrs.begin(), currentRepositoriesAddrs.end());

  // Get components of current repository

  ScAddrVector currentComponentsSpecificationsAddrs;

  try
  {
    currentComponentsSpecificationsAddrs = GetSpecificationsAddrs(
        context, repository, keynodes::ScComponentManagerKeynodes::rrel_components_specifications);
  }
  catch (utils::ScException const & exception)
  {
    SC_LOG_ERROR(exception.Message());
  }

  // Download specififcations

  for (ScAddr const & componentSpecificationAddr : currentComponentsSpecificationsAddrs)
  {
    downloaderHandler->Download(context, componentSpecificationAddr);
  }

  availableRepositories.pop_back();
  ProcessRepositories(context, availableRepositories);
}

/**
 * @brief Get all sc-addrs from first set
 * that connected with nodeAddr by rrelAddr relation.
 * @param context current sc-memory context
 * @param nodeAddr sc-addr of node that connected with set
 * @param rrelAddr sc-addr of relation that connects nodeAddr with set
 * @return Vector of all sc-addrs of all NodeConst nodes
 * that are in set that is connected with nodeAddr by rrelAdd rrelation.
 */
ScAddrVector ScComponentManagerCommandInit::GetSpecificationsAddrs(
    ScMemoryContext * context,
    ScAddr const & nodeAddr,
    ScAddr const & rrelAddr)
{
  ScAddr const & specificationsSetAddr = utils::IteratorUtils::getFirstByOutRelation(context, nodeAddr, rrelAddr);

  ScAddrVector specificationsAddrs =
      utils::IteratorUtils::getAllWithType(context, specificationsSetAddr, ScType::NodeConst);

  return specificationsAddrs;
}
