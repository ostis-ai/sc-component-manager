/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>

#include "sc_component_manager_command_search.hpp"

#include "module/keynodes/ScComponentManagerKeynodes.hpp"
#include "module/utils/common_utils.hpp"

using namespace common_utils;

ScAddrUnorderedSet ScComponentManagerCommandSearch::Execute(ScAgentContext * context, ScAddr const & actionAddr)
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
      ScType::VarNodeStructure >> SPECIFICATION_ALIAS,
      ScType::VarPermPosArc,
      ScType::VarNode >> COMPONENT_ALIAS,
      ScType::VarPermPosArc,
      ScKeynodes::rrel_key_sc_element);
  searchComponentTemplate.Triple(
      keynodes::ScComponentManagerKeynodes::concept_reusable_component, ScType::VarPermPosArc, COMPONENT_ALIAS);

  if (commandParameters.find(CLASS) != commandParameters.cend())
  {
    SearchComponentsByClass(context, searchComponentTemplate, commandParameters.at(CLASS));
  }

  for (auto const & [parameterName, parameterRelation, templateAlias] : searchByRelation)
  {
    if (commandParameters.find(parameterName) != commandParameters.cend())
    {
      SearchComponentsByRelation(
          context, parameterRelation, templateAlias, searchComponentTemplate, commandParameters.at(parameterName));
    }
  }

  for (auto const & [parameterName, parameterRelation, templateAlias] : searchByRelationSet)
  {
    if (commandParameters.find(parameterName) != commandParameters.cend())
    {
      SearchComponentsByRelationSet(
          context, parameterRelation, templateAlias, searchComponentTemplate, commandParameters.at(parameterName));
    }
  }

  std::map<std::string, ScAddrUnorderedSet> linksValues;
  for (auto const & [parameterName, parameterRelation, templateAlias] : searchByLine)
  {
    if (commandParameters.find(parameterName) != commandParameters.cend()
        && !commandParameters.find(parameterName)->second.empty())
    {
      ScAddrUnorderedSet links = SearchComponentsByRelationLink(
          context, parameterRelation, templateAlias, searchComponentTemplate, commandParameters.at(parameterName));
      linksValues.insert({templateAlias, links});
    }
  }

  ScAddrUnorderedSet componentsSpecifications =
      SearchComponentsSpecifications(context, searchComponentTemplate, linksValues);

  return componentsSpecifications;
}

void ScComponentManagerCommandSearch::SearchComponentsByRelation(
    ScMemoryContext * context,
    ScAddr const & relationAddr,
    std::string const & nodeAlias,
    ScTemplate & searchComponentTemplate,
    std::vector<std::string> const & parameters)
{
  if (parameters.empty())
    searchComponentTemplate.Quintuple(
        COMPONENT_ALIAS, ScType::VarCommonArc, ScType::VarNode >> nodeAlias, ScType::VarPermPosArc, relationAddr);
  for (std::string const & parameterIdentifier : parameters)
  {
    ScAddr parameterAddr = context->SearchElementBySystemIdentifier(parameterIdentifier);
    if (!parameterAddr.IsValid())
    {
      searchComponentTemplate.Clear();
      break;
    }
    searchComponentTemplate.Quintuple(
        COMPONENT_ALIAS, ScType::VarCommonArc, parameterAddr, ScType::VarPermPosArc, relationAddr);
  }
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
      ScType::VarCommonArc,
      ScType::VarNode >> setAlias,  // VarNodeTuple
      ScType::VarPermPosArc,
      relationAddr);
  for (std::string const & parameterIdentifier : parameters)
  {
    ScAddr parameterAddr = context->SearchElementBySystemIdentifier(parameterIdentifier);
    if (!parameterAddr.IsValid())
    {
      searchComponentTemplate.Clear();
      break;
    }
    searchComponentTemplate.Triple(setAlias, ScType::VarPermPosArc, parameterAddr);
  }
}

void ScComponentManagerCommandSearch::SearchComponentsByClass(
    ScMemoryContext * context,
    ScTemplate & searchComponentTemplate,
    std::vector<std::string> const & parameters)
{
  for (std::string const & classIdentifier : parameters)
  {
    ScAddr classAddr = context->SearchElementBySystemIdentifier(classIdentifier);
    if (!classAddr.IsValid())
    {
      searchComponentTemplate.Clear();
      break;
    }
    searchComponentTemplate.Triple(classAddr, ScType::VarPermPosArc, COMPONENT_ALIAS);
  }
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
