/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager_command_search.hpp"

#include <sc-agents-common/utils/IteratorUtils.hpp>

#include <algorithm>

#include <common/sc_component_manager_keynodes.hpp>
#include <common/common_utils.hpp>

using namespace common_utils;

ScAddrUnorderedSet ScComponentManagerCommandSearch::Execute(ScAgentContext * context, ScAddr const & actionAddr)
{
  ScAddrUnorderedSet componentsSpecifications;
  ScAddr const searchStructure = utils::IteratorUtils::getAnyByOutRelation(context, actionAddr, ScKeynodes::rrel_1);

  ScTemplate searchTemplate;
  context->BuildTemplate(searchTemplate, searchStructure);

  context->SearchByTemplate(searchTemplate,
  [&context, searchStructure](ScTemplateResultItem const & item) 
  {
    ScIterator5Ptr const reusableComponentIterator = context->Iterator5(searchStructure, ScType::VarPermPosArc, keynodes::ScComponentManagerKeynodes::concept_reusable_component, ScType::VarPermPosArc, ScType::VarNode);
    if (reusableComponentIterator->Next())
    {

    }
  },
  [&context](ScAddr const & item) 
  {
    return false;
  });

  return componentsSpecifications;
}

ScAddrUnorderedSet ScComponentManagerCommandSearch::SearchComponentsByRelationLink(
    ScMemoryContext * context,
    ScAddr const & relationAddr,
    std::string const & linkAlias,
    ScTemplate & searchComponentTemplate,
    std::vector<std::string> & parameters)
{
  if (parameters.size() > 1)
  {
    for (size_t sequenceNumber = 1; sequenceNumber < parameters.size(); sequenceNumber++)
    {
      parameters[0] += " ";
      parameters[0] += parameters[sequenceNumber];
    }
    parameters.erase(parameters.begin() + 1, parameters.end());
    SC_LOG_INFO(
        "ScComponentManagerCommandSearch: Unsupported multiple links search, the search was performed across the full "
        "line");
  }
  ScAddrSet links = context->SearchLinksByContentSubstring(parameters.at(0));

  searchComponentTemplate.Quintuple(
      COMPONENT_ALIAS, ScType::VarCommonArc, ScType::VarNodeLink >> linkAlias, ScType::VarPermPosArc, relationAddr);

  ScAddrUnorderedSet result(links.begin(), links.end());
  return result;
}

ScAddrUnorderedSet ScComponentManagerCommandSearch::SearchComponentsSpecifications(
    ScMemoryContext * context,
    ScTemplate & searchComponentTemplate,
    std::map<std::string, ScAddrUnorderedSet> const & linksValues)
{
  ScAddrUnorderedSet result;
  ScTemplateSearchResult searchComponentResult;
  context->SearchByTemplate(searchComponentTemplate, searchComponentResult);

  if (linksValues.empty())
  {
    result = SearchComponentsSpecificationsWithoutLinks(context, searchComponentResult);
  }
  else
  {
    result = SearchComponentsSpecificationsWithLinks(context, searchComponentResult, linksValues);
  }

  return result;
}

ScAddrUnorderedSet ScComponentManagerCommandSearch::SearchComponentsSpecificationsWithoutLinks(
    ScMemoryContext * context,
    ScTemplateSearchResult const & searchComponentResult)
{
  ScAddrUnorderedSet result;
  for (size_t i = 0; i < searchComponentResult.Size(); i++)
  {
    ScAddr reusableComponentSpecification = searchComponentResult[i][SPECIFICATION_ALIAS];
    ScAddr reusableComponent = searchComponentResult[i][COMPONENT_ALIAS];
    SC_LOG_INFO(
        "ScComponentManager: specification is " << context->GetElementSystemIdentifier(reusableComponentSpecification)
                                                << ", component is "
                                                << context->GetElementSystemIdentifier(reusableComponent));
    result.insert(reusableComponentSpecification);
  }

  return result;
}

ScAddrUnorderedSet ScComponentManagerCommandSearch::SearchComponentsSpecificationsWithLinks(
    ScMemoryContext * context,
    ScTemplateSearchResult const & searchComponentResult,
    std::map<std::string, ScAddrUnorderedSet> const & linksValues)
{
  ScAddrUnorderedSet result;
  for (size_t i = 0; i < searchComponentResult.Size(); i++)
  {
    for (auto const & linkValue : linksValues)
    {
      for (ScAddr const & link : linkValue.second)
      {
        if (link == searchComponentResult[i][linkValue.first])
        {
          ScAddr reusableComponentSpecification = searchComponentResult[i][SPECIFICATION_ALIAS];
          ScAddr reusableComponent = searchComponentResult[i][COMPONENT_ALIAS];
          SC_LOG_INFO(
              "ScComponentManager: specification is "
              << context->GetElementSystemIdentifier(reusableComponentSpecification) << ", component is "
              << context->GetElementSystemIdentifier(reusableComponent));
          result.insert(reusableComponentSpecification);
        }
      }
    }
  }

  return result;
}
