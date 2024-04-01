/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "downloader/downloader.hpp"
#include "sc_component_manager_command_init.hpp"
#include "utils/sc_component_utils.hpp"

ScAddrVector ScComponentManagerCommandInit::Execute(ScMemoryContext * context, ScAddr const & actionAddr)
{
  ScAddrVector processedRepositories;

  ScAddrVector availableRepositories = utils::IteratorUtils::getAllWithType(
      context, keynodes::ScComponentManagerKeynodes::concept_repository, ScType::NodeConst);

  ProcessRepositories(context, availableRepositories);

  ScAddrVector components;
  ScIterator3Ptr const & componentsIterator = context->Iterator3(
      keynodes::ScComponentManagerKeynodes::concept_reusable_component,
      ScType::EdgeAccessConstPosPerm,
      ScType::NodeConst);
  while (componentsIterator->Next())
  {
    SC_LOG_DEBUG(context->HelperGetSystemIdtf(componentsIterator->Get(2)));
    components.push_back(componentsIterator->Get(2));
  }

  return components;
}

/**
 * @brief Recursivly iterates through repositories
 * and download avaible components specifications.
 * @param context current sc-memory context
 * @param avaibleRepositories vector of avaible repositories addrs
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
    downloaderHandler->DownloadSpecification(context, componentSpecificationAddr);
    std::string const specificationPath = m_specificationsPath + SpecificationConstants::DIRECTORY_DELIMITER
                                          + context->HelperGetSystemIdtf(componentSpecificationAddr);
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
 * that are in set that is connected with nodeAddr by rrelAdd rrelation.
 */
ScAddrVector ScComponentManagerCommandInit::GetSpecificationsAddrs(
    ScMemoryContext * context,
    ScAddr const & nodeAddr,
    ScAddr const & rrelAddr)
{
  ScAddr const & specificationsSetAddr = utils::IteratorUtils::getAnyByOutRelation(context, nodeAddr, rrelAddr);

  ScAddrVector specificationsAddrs =
      utils::IteratorUtils::getAllWithType(context, specificationsSetAddr, ScType::NodeConst);

  return specificationsAddrs;
}
