/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>

#include "sc_component_manager_command_search.hpp"
#include "../../common-module/module/keynodes/ScComponentManagerKeynodes.hpp"
#include "../../common-module/module/utils/common_utils.hpp"

ScAddrVector ScComponentManagerCommandSearch::Execute(ScMemoryContext * context, ScAddr const & actionAddr)
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
  searchComponentTemplate.Triple(
      keynodes::ScComponentManagerKeynodes::concept_reusable_component,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> COMPONENT_ALIAS);

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

  std::map<std::string, ScAddrVector> linksValues;
  if (commandParameters.find(EXPLANATION) != commandParameters.cend()
      && !commandParameters.find(EXPLANATION)->second.empty())
  {
    ScAddrVector explanationLinks = SearchComponentsByRelationLink(
        context,
        keynodes::ScComponentManagerKeynodes::nrel_explanation,
        EXPLANATION_LINK_ALIAS,
        searchComponentTemplate,
        commandParameters.at(EXPLANATION));
    linksValues.insert({EXPLANATION_LINK_ALIAS, explanationLinks});
  }

  std::vector<std::string> componentsIdtfs;
  componentsIdtfs = SearchComponents(context, searchComponentTemplate, linksValues);
  ScAddrVector components;
  ScAddr component;
  for (std::string const & componentIdtf : componentsIdtfs)
  {
    component = context->HelperFindBySystemIdtf(componentIdtf);
    ScIterator3Ptr itSpecification =
        context->Iterator3(ScType::NodeConstStruct, ScType::EdgeAccessConstPosPerm, component);
    while (itSpecification->Next())
    {
      if (context->HelperCheckEdge(
              keynodes::ScComponentManagerKeynodes::concept_reusable_component_specification,
              itSpecification->Get(0),
              ScType::EdgeAccessConstPosPerm))
      {
        SC_LOG_INFO("ScComponentManager: " + context->HelperGetSystemIdtf(itSpecification->Get(0)) + " " + componentIdtf);
        components.push_back(itSpecification->Get(0));
        break;
      }
    }
  }

  return components;
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

ScAddrVector ScComponentManagerCommandSearch::SearchComponentsByRelationLink(
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

  return links;
}

std::vector<std::string> ScComponentManagerCommandSearch::SearchComponents(
    ScMemoryContext * context,
    ScTemplate & searchComponentTemplate,
    std::map<std::string, ScAddrVector> const & linksValues)
{
  std::vector<std::string> result;
  ScTemplateSearchResult searchComponentResult;
  context->HelperSearchTemplate(searchComponentTemplate, searchComponentResult);

  if (linksValues.empty())
  {
    result = SearchComponentsWithoutLinks(context, searchComponentResult);
  }
  else
  {
    result = SearchComponentsWithLinks(context, searchComponentResult, linksValues);
  }

  return result;
}

std::vector<std::string> ScComponentManagerCommandSearch::SearchComponentsWithoutLinks(
    ScMemoryContext * context,
    ScTemplateSearchResult const & searchComponentResult)
{
  std::vector<std::string> result;
  for (size_t i = 0; i < searchComponentResult.Size(); i++)
  {
    ScAddr reusableComponent = searchComponentResult[i][COMPONENT_ALIAS];
    result.push_back(context->HelperGetSystemIdtf(reusableComponent));
  }

  return result;
}

std::vector<std::string> ScComponentManagerCommandSearch::SearchComponentsWithLinks(
    ScMemoryContext * context,
    ScTemplateSearchResult const & searchComponentResult,
    std::map<std::string, ScAddrVector> const & linksValues)
{
  std::vector<std::string> result;
  for (size_t i = 0; i < searchComponentResult.Size(); i++)
  {
    for (auto const & linkValue : linksValues)
    {
      for (ScAddr const & link : linkValue.second)
      {
        if (link == searchComponentResult[i][linkValue.first])
        {
          ScAddr reusableComponent = searchComponentResult[i][COMPONENT_ALIAS];
          result.push_back(context->HelperGetSystemIdtf(reusableComponent));
        }
      }
    }
  }

  return result;
}
