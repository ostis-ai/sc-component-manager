/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "downloader/downloader.hpp"
#include "sc_component_manager_command_init.hpp"
#include "utils/sc_component_utils.hpp"
#include "module/utils/common_utils.hpp"

using namespace common_utils;

ScAddrUnorderedSet ScComponentManagerCommandInit::Execute(ScAgentContext * context, ScAddr const & actionAddr)
{
  ScAddrUnorderedSet specifications;

  ScIterator3Ptr const repositoriesIterator = context->CreateIterator3(
      keynodes::ScComponentManagerKeynodes::concept_repository, ScType::ConstPermPosArc, ScType::ConstNode);

  ScAddr repository;
  while (repositoriesIterator->Next())
  {
    repository = repositoriesIterator->Get(2);
    ProcessRepository(context, repository, specifications);
  }

  return specifications;
}

/**
 * @brief Download available components specifications from a storage.
 * @param context current sc-memory context
 * @param repository ScAddr of a repository of components to load
 */
void ScComponentManagerCommandInit::ProcessRepository(
    ScMemoryContext * context,
    ScAddr & repository,
    ScAddrUnorderedSet & specifications)
{
  ScAddr const & specificationsSetAddr = utils::IteratorUtils::getAnyByOutRelation(
      context, repository, keynodes::ScComponentManagerKeynodes::rrel_components_specifications);
  if (!context->IsElement(specificationsSetAddr))
  {
    SC_LOG_WARNING(
        "ScComponentManagerCommandInit: components specification not found in repository "
        << context->GetElementSystemIdentifier(repository));
    return;
  }

  ScAddr componentSpecification;
  ScAddr component;
  ScIterator3Ptr const specificationsIterator =
      context->CreateIterator3(specificationsSetAddr, ScType::ConstPermPosArc, ScType::ConstNode);
  while (specificationsIterator->Next())
  {
    componentSpecification = specificationsIterator->Get(2);
    component = CommonUtils::GetComponentBySpecification(*context, componentSpecification);
    if (context->IsElement(component))
    {
      SC_LOG_WARNING(
          "ScComponentManagerCommandInit: Specification is already loaded for component "
          << context->GetElementSystemIdentifier(component));
      continue;
    }
    specifications.insert(componentSpecification);
    downloaderHandler->DownloadSpecification(context, componentSpecification);
    std::string const specificationPath = m_specificationsPath + SpecificationConstants::DIRECTORY_DELIMITER
                                          + context->GetElementSystemIdentifier(componentSpecification);
    componentUtils::LoadUtils::LoadScsFilesInDir(context, specificationPath);
    SC_LOG_DEBUG(
        "ScComponentManagerCommandInit: loaded specification "
        << context->GetElementSystemIdentifier(componentSpecification));
  }
}
