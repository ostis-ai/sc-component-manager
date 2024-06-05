/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>

#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "sc_component_manager_command_search.hpp"
#include "module/keynodes/ScComponentManagerKeynodes.hpp"

common_utils::CommonUtils::ScAddrUnorderedSet ScComponentManagerCommandSearch::Execute(
    ScMemoryContext * context,
    ScAddr const & actionAddr)
{
  std::map<std::string, std::vector<std::string>> commandParameters =
      common_utils::CommonUtils::GetCommandParameters(*context, actionAddr);
  for (auto const & param : commandParameters)
  {
    if (std::find(possibleSearchParameters.cbegin(), possibleSearchParameters.cend(), param.first)
        == possibleSearchParameters.cend())
    {
      SC_THROW_EXCEPTION(
          utils::ExceptionParseError, "ScComponentManagerCommandSearch: Unsupported search parameter " << param.first);
    }
  }

  ScTemplate searchComponentTemplate;
  searchComponentTemplate.Quintuple(
      ScType::NodeVarStruct >> SPECIFICATION_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> COMPONENT_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      scAgentsCommon::CoreKeynodes::rrel_key_sc_element);
  searchComponentTemplate.Triple(
      keynodes::ScComponentManagerKeynodes::concept_reusable_component, ScType::EdgeAccessVarPosPerm, COMPONENT_ALIAS);

  if (commandParameters.find(CLASS) != commandParameters.cend())
  {
    SearchComponentsByClass(context, searchComponentTemplate, commandParameters.at(CLASS));
  }

  if (commandParameters.find(AUTHOR) != commandParameters.cend())
  {
    SearchComponentsByRelationSet(
        context,
        keynodes::ScComponentManagerKeynodes::nrel_authors,
        AUTHORS_SET_ALIAS,
        searchComponentTemplate,
        commandParameters.at(AUTHOR));
  }

  std::map<std::string, common_utils::CommonUtils::ScAddrUnorderedSet> linksValues;
  if (commandParameters.find(EXPLANATION) != commandParameters.cend()
      && !commandParameters.find(EXPLANATION)->second.empty())
  {
    common_utils::CommonUtils::ScAddrUnorderedSet explanationLinks = SearchComponentsByRelationLink(
        context,
        keynodes::ScComponentManagerKeynodes::nrel_explanation,
        EXPLANATION_LINK_ALIAS,
        searchComponentTemplate,
        commandParameters.at(EXPLANATION));
    linksValues.insert({EXPLANATION_LINK_ALIAS, explanationLinks});
  }

  common_utils::CommonUtils::ScAddrUnorderedSet componentsSpecifications =
      SearchComponentsSpecifications(context, searchComponentTemplate, linksValues);

  return componentsSpecifications;
}

void ScComponentManagerCommandSearch::SearchComponentsByRelationSet(
    ScMemoryContext * context,
    ScAddr const & relationAddr,
    std::string const & setAlias,
    ScTemplate & searchComponentTemplate,
    std::vector<std::string> const & parameters)
{
  searchComponentTemplate.Quintuple(
      COMPONENT_ALIAS,
      ScType::EdgeDCommonVar,
      ScType::NodeVar >> setAlias,  // NodeVarTuple
      ScType::EdgeAccessVarPosPerm,
      relationAddr);
  for (std::string const & parameterIdentifier : parameters)
  {
    ScAddr parameterAddr = context->HelperFindBySystemIdtf(parameterIdentifier);
    if (!parameterAddr.IsValid())
    {
      searchComponentTemplate.Clear();
      break;
    }
    searchComponentTemplate.Triple(setAlias, ScType::EdgeAccessVarPosPerm, parameterAddr);
  }
}

void ScComponentManagerCommandSearch::SearchComponentsByClass(
    ScMemoryContext * context,
    ScTemplate & searchComponentTemplate,
    std::vector<std::string> const & parameters)
{
  for (std::string const & classIdentifier : parameters)
  {
    ScAddr classAddr = context->HelperFindBySystemIdtf(classIdentifier);
    if (!classAddr.IsValid())
    {
      searchComponentTemplate.Clear();
      break;
    }
    searchComponentTemplate.Triple(classAddr, ScType::EdgeAccessVarPosPerm, COMPONENT_ALIAS);
  }
}

common_utils::CommonUtils::ScAddrUnorderedSet ScComponentManagerCommandSearch::SearchComponentsByRelationLink(
    ScMemoryContext * context,
    ScAddr const & relationAddr,
    std::string const & linkAlias,
    ScTemplate & searchComponentTemplate,
    std::vector<std::string> const & parameters)
{
  if (parameters.size() > 1)
  {
    SC_THROW_EXCEPTION(
        utils::ExceptionParseError, "ScComponentManagerCommandSearch: Unsupported multiple links search");
  }
  ScAddrVector links = context->FindLinksByContentSubstring(parameters.at(0));

  searchComponentTemplate.Quintuple(
      COMPONENT_ALIAS,
      ScType::EdgeDCommonVar,
      ScType::LinkVar >> linkAlias,
      ScType::EdgeAccessVarPosPerm,
      relationAddr);

  common_utils::CommonUtils::ScAddrUnorderedSet result(links.begin(), links.end());
  return result;
}

common_utils::CommonUtils::ScAddrUnorderedSet ScComponentManagerCommandSearch::SearchComponentsSpecifications(
    ScMemoryContext * context,
    ScTemplate & searchComponentTemplate,
    std::map<std::string, common_utils::CommonUtils::ScAddrUnorderedSet> const & linksValues)
{
  common_utils::CommonUtils::ScAddrUnorderedSet result;
  ScTemplateSearchResult searchComponentResult;
  context->HelperSearchTemplate(searchComponentTemplate, searchComponentResult);

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

common_utils::CommonUtils::ScAddrUnorderedSet ScComponentManagerCommandSearch::
    SearchComponentsSpecificationsWithoutLinks(
        ScMemoryContext * context,
        ScTemplateSearchResult const & searchComponentResult)
{
  common_utils::CommonUtils::ScAddrUnorderedSet result;
  for (size_t i = 0; i < searchComponentResult.Size(); i++)
  {
    ScAddr reusableComponentSpecification = searchComponentResult[i][SPECIFICATION_ALIAS];
    ScAddr reusableComponent = searchComponentResult[i][COMPONENT_ALIAS];
    SC_LOG_INFO(
        "ScComponentManager: specification is " << context->HelperGetSystemIdtf(reusableComponentSpecification)
                                                << ", component is "
                                                << context->HelperGetSystemIdtf(reusableComponent));
    result.insert(reusableComponentSpecification);
  }

  return result;
}

common_utils::CommonUtils::ScAddrUnorderedSet ScComponentManagerCommandSearch::SearchComponentsSpecificationsWithLinks(
    ScMemoryContext * context,
    ScTemplateSearchResult const & searchComponentResult,
    std::map<std::string, common_utils::CommonUtils::ScAddrUnorderedSet> const & linksValues)
{
  common_utils::CommonUtils::ScAddrUnorderedSet result;
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
              "ScComponentManager: specification is " << context->HelperGetSystemIdtf(reusableComponentSpecification)
                                                      << ", component is "
                                                      << context->HelperGetSystemIdtf(reusableComponent));
          result.insert(reusableComponentSpecification);
        }
      }
    }
  }

  return result;
}
