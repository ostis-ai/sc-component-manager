/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>

#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "sc_component_manager_command_search.hpp"

#include "module/keynodes/ScComponentManagerKeynodes.hpp"
#include "module/utils/common_utils.hpp"

using namespace common_utils;

ScAddrUnorderedSet ScComponentManagerCommandSearch::Execute(ScMemoryContext * context, ScAddr const & actionAddr)
{
  std::map<std::string, std::vector<std::string>> commandParameters =
      CommonUtils::GetCommandParameters(*context, actionAddr);
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

  if (commandParameters.find(KEY) != commandParameters.cend())
  {
    SearchComponentsByRelationSet(
        context,
        keynodes::ScComponentManagerKeynodes::nrel_key_sc_element,
        KEY_SET_ALIAS,
        searchComponentTemplate,
        commandParameters.at(KEY));
  }

  std::map<std::string, ScAddrUnorderedSet> linksValues;
  if (commandParameters.find(EXPLANATION) != commandParameters.cend()
      && !commandParameters.find(EXPLANATION)->second.empty())
  {
    ScAddrUnorderedSet explanationLinks = SearchComponentsByRelationLink(
        context,
        keynodes::ScComponentManagerKeynodes::nrel_explanation,
        EXPLANATION_LINK_ALIAS,
        searchComponentTemplate,
        commandParameters.at(EXPLANATION));
    linksValues.insert({EXPLANATION_LINK_ALIAS, explanationLinks});
  }
  
  if (commandParameters.find(NOTE) != commandParameters.cend()
      && !commandParameters.find(NOTE)->second.empty())
  {
    ScAddrUnorderedSet noteLinks = SearchComponentsByRelationLink(
        context,
        keynodes::ScComponentManagerKeynodes::nrel_note,
        NOTE_LINK_ALIAS,
        searchComponentTemplate,
        commandParameters.at(NOTE));
    linksValues.insert({NOTE_LINK_ALIAS, noteLinks});
  }

  if (commandParameters.find(PURPOSE) != commandParameters.cend()
      && !commandParameters.find(PURPOSE)->second.empty())
  {
    ScAddrUnorderedSet purposeLinks = SearchComponentsByRelationLink(
        context,
        keynodes::ScComponentManagerKeynodes::nrel_purpose,
        PURPOSE_LINK_ALIAS,
        searchComponentTemplate,
        commandParameters.at(PURPOSE));
    linksValues.insert({PURPOSE_LINK_ALIAS, purposeLinks});
  }

  if (commandParameters.find(MAIN_ID) != commandParameters.cend()
      && !commandParameters.find(MAIN_ID)->second.empty())
  {
    ScAddrUnorderedSet idLinks = SearchComponentsByRelationLink(
        context,
        scAgentsCommon::CoreKeynodes::nrel_main_idtf,
        ID_LINK_ALIAS,
        searchComponentTemplate,
        commandParameters.at(MAIN_ID));
    linksValues.insert({ID_LINK_ALIAS, idLinks});
  }

  ScAddrUnorderedSet componentsSpecifications =
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

ScAddrUnorderedSet ScComponentManagerCommandSearch::SearchComponentsByRelationLink(
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
        "ScComponentManager: specification is " << context->HelperGetSystemIdtf(reusableComponentSpecification)
                                                << ", component is "
                                                << context->HelperGetSystemIdtf(reusableComponent));
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
