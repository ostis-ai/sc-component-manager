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

  // Get all avaible repositories for current repository
  ScAddrVector currentRepositoriesAddrs = GetSpecificationsAddrs(
      context, repository, keynodes::ScComponentManagerKeynodes::rrel_repositories_specifications);

  // Remove processed repository from avaible
  availableRepositories.pop_back();

  // Add found repositories to avaible repositories
  availableRepositories.insert(
      availableRepositories.end(), currentRepositoriesAddrs.begin(), currentRepositoriesAddrs.end());

  // Get components of current repository
  ScAddrVector currentComponentsSpecificationsAddrs =
      GetSpecificationsAddrs(context, repository, keynodes::ScComponentManagerKeynodes::rrel_components_specifications);

  // Download specififcations

  for (ScAddr const & componentSpecificationAddr : currentComponentsSpecificationsAddrs)
  {
    // DownloadSpecification(context, componentSpecificationAddr);
  }
}

void DownloadSpecification(ScMemoryContext * context, ScAddr const & componentSpecificationAddr)
{
  // Get alternative addresses set addr
  ScIterator3Ptr alternativeAddressesSetIterator =
      context->Iterator3(componentSpecificationAddr, ScType::EdgeAccessConstPosPerm, ScType::NodeTuple);
  if (!alternativeAddressesSetIterator->Next())
  {
    SC_LOG_ERROR("No alternative addresses set found");
    return;
  }
  ScAddr const & alternativeAddressesSet = alternativeAddressesSetIterator->Get(2);

  if (utils::CommonUtils::isEmpty(context, alternativeAddressesSet))
  {
    SC_LOG_ERROR("Alternative addresses set is empty");
    return;
  }

  // Get address from alternative addresses set
  ScAddr specificationAddressAddr = utils::IteratorUtils::getFirstFromSet(context, alternativeAddressesSet, true);

  if (!specificationAddressAddr.IsValid())
  {
    specificationAddressAddr = utils::IteratorUtils::getAnyFromSet(context, alternativeAddressesSet);
  }

  // Get links with address
  ScAddrVector specificationAddressLinks = utils::IteratorUtils::getAllWithType(context, specificationAddressAddr, ScType::LinkConst);

  // for (ScAddr const & specificationLink: specificationAddressLinks)
  // {
  //   DownloadByUrl();
  //   UploadInMemory();
  // }
};

/**
 * @brief Get all address from first set
 * that connected with nodeAddr by rrelAddr relation.
 * @param context current sc-memory context
 * @param nodeAddr ScAddr of node that connected with set
 * @param rrelAddr ScAddr of relation that connects nodeAddr with set
 * @return Vector of all sc-addrs of all NodeConst nodes that are in set that is connected with nodeAddr by rrelAddr
 * relation.
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
