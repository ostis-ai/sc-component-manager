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

bool ScComponentManagerCommandInit::Execute(ScMemoryContext * context, CommandParameters const & commandParameters)
{
  ScAddrVector processedRepositories;

  ScAddrVector availableRepositories = utils::IteratorUtils::getAllWithType(
      context, keynodes::ScComponentManagerKeynodes::concept_repository, ScType::NodeConst);

  bool executionResult = ProcessRepositories(context, availableRepositories);

  return executionResult;
}

/**
 * @brief Recursively iterates through repositories
 * and download available components specifications.
 * @param context current sc-memory context
 * @param availableRepositories vector of available repositories addrs
 */
bool ScComponentManagerCommandInit::ProcessRepositories(ScMemoryContext * context, ScAddrVector & availableRepositories)
{
  if (availableRepositories.empty())
    return true;

  ScAddr const repository = availableRepositories.back();

  ScAddrVector currentRepositoriesAddrs;
  try
  {
    currentRepositoriesAddrs = GetSpecificationsAddrs(
        context, repository, keynodes::ScComponentManagerKeynodes::rrel_repositories_specifications);
  }
  catch (utils::ScException const & exception)
  {
    SC_LOG_DEBUG("ScComponentManagerCommandInit: Problem getting repositories specifications");
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
    SC_LOG_DEBUG("ScComponentManagerCommandInit: Problem getting component specifications");
    SC_LOG_DEBUG(exception.Message());
  }

  for (ScAddr const & componentSpecificationAddr : currentComponentsSpecificationsAddrs)
  {
    downloaderHandler->Download(context, componentSpecificationAddr);
    std::string const specificationPath = m_specificationsPath + SpecificationConstants::DIRECTORY_DELIMETR +
                                          context->HelperGetSystemIdtf(componentSpecificationAddr);
    componentUtils::LoadUtils::LoadScsFilesInDir(context, specificationPath);

    ScAddrVector componentDependencies =
        componentUtils::SearchUtils::GetComponentDependencies(context, componentSpecificationAddr);
    availableRepositories.insert(
        availableRepositories.end(), componentDependencies.begin(), componentDependencies.end());
  }

  availableRepositories.pop_back();
  ProcessRepositories(context, availableRepositories);

  return true;
}

/**
 * @brief Get all sc-addrs from first set
 * that connected with nodeAddr by rrelAddr relation.
 * @param context current sc-memory context
 * @param nodeAddr sc-addr of node that connected with set
 * @param rrelAddr sc-addr of relation that connects nodeAddr with set
 * @return Vector of all sc-addrs of all NodeConst nodes
 * that are in set that is connected with nodeAddr by rrelAdd relation.
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
