/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-agents-common/utils/IteratorUtils.hpp>

#include "src/manager/commands/sc_component_manager_command.hpp"
#include "src/manager/downloader/downloader_handler.hpp"
#include "src/manager/commands/command_init/sc_component_manager_command_init.hpp"
#include "src/manager/utils/sc_component_utils.hpp"

ExecutionResult ScComponentManagerCommandInit::Execute(
    ScMemoryContext * context,
    CommandParameters const & commandParameters)
{
  ScAddrVector processedRepositories;

  ScAddrVector availableRepositories = utils::IteratorUtils::getAllWithType(
      context, keynodes::ScComponentManagerKeynodes::concept_repository, ScType::NodeConst);

  ProcessRepositories(context, availableRepositories);

  ExecutionResult executionResult;

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

  ScAddr const repository = availableRepositories.back();

  ScAddrVector currentRepositoriesAddrs;
  try
  {
    currentRepositoriesAddrs = GetSpecificationsAddrs(
        context, repository, keynodes::ScComponentManagerKeynodes::rrel_repositories_specifications);
  }
  catch (utils::ScException const & exception)
  {
    SC_LOG_DEBUG("Problem getting repositories specifications");
    SC_LOG_DEBUG(exception.Message());
  }

  availableRepositories.insert(
      availableRepositories.begin(), currentRepositoriesAddrs.begin(), currentRepositoriesAddrs.end());

  ScAddrVector currentComponentsSpecificationsAddrs;
  try
  {
    currentComponentsSpecificationsAddrs = GetSpecificationsAddrs(
        context, repository, keynodes::ScComponentManagerKeynodes::rrel_components_specifications);
  }
  catch (utils::ScException const & exception)
  {
    SC_LOG_DEBUG("Problem getting component specifications");
    SC_LOG_DEBUG(exception.Message());
  }

  for (ScAddr const & componentSpecificationAddr : currentComponentsSpecificationsAddrs)
  {
    downloaderHandler->Download(context, componentSpecificationAddr);
    std::string const specificationPath = m_specificationsPath + SpecificationConstants::DIRECTORY_DELIMETR +
                                          context->HelperGetSystemIdtf(componentSpecificationAddr);
    componentUtils::LoadUtils::LoadScsFilesInDir(context, specificationPath);

    ScAddrVector componentDependencies = componentUtils::SearchUtils::GetComponentDependencies(context, componentSpecificationAddr);
    ProcessRepositories(context, componentDependencies);
  }

  availableRepositories.pop_back();
  ProcessRepositories(context, availableRepositories); // need to understand this logic and rewrite it
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
