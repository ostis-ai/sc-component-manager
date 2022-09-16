/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <utility>
#include <algorithm>

#include "src/manager/commands/sc_component_manager_command.hpp"
#include "src/manager/commands/keynodes/ScComponentManagerKeynodes.hpp"

class ScComponentManagerCommandSearch : public ScComponentManagerCommand
{
public:
  ScComponentManagerCommandSearch() = default;

  ExecutionResult Execute(ScMemoryContext * context, CommandParameters const & commandParameters) override
  {
    //
    for (auto const & param : commandParameters)
    {
      std::cout << "Param type: " << param.first << "\nParam value: ";
      if (std::find(possibleSearchParameters.cbegin(), possibleSearchParameters.cend(), param.first) ==
          possibleSearchParameters.cend())
      {
        SC_THROW_EXCEPTION(
            utils::ExceptionParseError,
            "ScComponentManagerCommandSearch: Unsupported search parameter " << param.first);
      }
      for (std::string const & val : param.second)
      {
        std::cout << val << " ";
      }
      std::cout << '\n';
    }
    // remove debug

    ScTemplate searchComponentTemplate;
    ScTemplateSearchResult searchComponentResult;
    searchComponentTemplate.Triple(
        keynodes::ScComponentManagerKeynodes::concept_reusable_component,
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> COMPONENT_ALIAS);

    if (commandParameters.find(AUTHOR) != commandParameters.cend())
    {
      std::cout << "Find by author\n";
      std::vector<std::string> const & parameters = commandParameters.at(AUTHOR);
      SearchComponentsByAuthor(context, searchComponentTemplate, parameters);
    }

    if (commandParameters.find(CLASS) != commandParameters.cend())
    {
      std::cout << "Find by class\n";
      SearchComponentsByClass(context, searchComponentTemplate, commandParameters.at(CLASS));
    }

    ScAddrVector links;
    if (commandParameters.find(EXPLANATION) != commandParameters.cend())
    {
      std::cout << "Find by explanation\n";
      links = SearchComponentsByExplanation(context, searchComponentTemplate, commandParameters.at(EXPLANATION));
    }

    ExecutionResult result;
    if (!links.empty())
    {
      result = SearchComponents(context, searchComponentTemplate, searchComponentResult, links);
    }
    else
    {
      result = SearchComponents(context, searchComponentTemplate, searchComponentResult);
    }

    return result;
  }

protected:
  std::string const COMPONENT_ALIAS = "_component";
  std::string const AUTHORS_SET_ALIAS = "_authors";
  std::string const EXPLANATION_ALIAS = "_explanation";

  std::string const AUTHOR = "author";
  std::string const CLASS = "class";
  std::string const EXPLANATION = "explanation";
  std::set<std::string> const possibleSearchParameters = {AUTHOR, CLASS, EXPLANATION};  // extend to more params

  void SearchComponentsByAuthor(
      ScMemoryContext * context,
      ScTemplate & searchComponentTemplate,
      std::vector<std::string> const & parameters)
  {
    searchComponentTemplate.TripleWithRelation(
        COMPONENT_ALIAS,
        ScType::EdgeDCommonVar,
        ScType::NodeVar >> AUTHORS_SET_ALIAS,  // NodeVarTuple
        ScType::EdgeAccessVarPosPerm,
        keynodes::ScComponentManagerKeynodes::nrel_author);
    for (std::string const & authorIdentifier : parameters)
    {
      ScAddr authorAddr = context->HelperFindBySystemIdtf(authorIdentifier);
      if (!authorAddr.IsValid())
      {
        searchComponentTemplate.Clear();
        break;
      }
      std::cout << "Found author: " << context->HelperGetSystemIdtf(authorAddr) << std::endl;
      searchComponentTemplate.Triple(AUTHORS_SET_ALIAS, ScType::EdgeAccessVarPosPerm, authorAddr);
    }
  }

  void SearchComponentsByClass(
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
      std::cout << "Found class: " << context->HelperGetSystemIdtf(classAddr) << std::endl;
      searchComponentTemplate.Triple(classAddr, ScType::EdgeAccessVarPosPerm, COMPONENT_ALIAS);
    }
  }

  ScAddrVector SearchComponentsByExplanation(
      ScMemoryContext * context,
      ScTemplate & searchComponentTemplate,
      std::vector<std::string> const & parameters)
  {
    if (parameters.size() > 1)
    {
      SC_THROW_EXCEPTION(
          utils::ExceptionParseError, "ScComponentManagerCommandSearch: Unsupported multiple explanation search");
    }
    ScAddrVector links = context->FindLinksByContentSubstring(parameters.at(0));

    searchComponentTemplate.TripleWithRelation(
        COMPONENT_ALIAS,
        ScType::EdgeDCommonVar,
        ScType::LinkVar >> EXPLANATION_ALIAS,
        ScType::EdgeAccessVarPosPerm,
        keynodes::ScComponentManagerKeynodes::nrel_explanation);

    return links;
  }

  ExecutionResult SearchComponents(ScMemoryContext * context, ScTemplate & searchComponentTemplate, ScTemplateSearchResult & searchComponentResult)
  {
    ExecutionResult result;
    if (!searchComponentTemplate.IsEmpty())
    {
      context->HelperSearchTemplate(searchComponentTemplate, searchComponentResult);
      for (size_t i = 0; i < searchComponentResult.Size(); i++)
      {
        ScAddr reusableComponent = searchComponentResult[i][COMPONENT_ALIAS];
        result.push_back(context->HelperGetSystemIdtf(reusableComponent));
      }
    }

    return result;
  }

  ExecutionResult SearchComponents(ScMemoryContext * context, ScTemplate & searchComponentTemplate, ScTemplateSearchResult & searchComponentResult, ScAddrVector const & links)
  {
    ExecutionResult result;
    for (ScAddr const & link : links)
    {
      context->HelperSearchTemplate(searchComponentTemplate, searchComponentResult);
      for (size_t i = 0; i < searchComponentResult.Size(); i++)
      {
        if (link == searchComponentResult[i][EXPLANATION_ALIAS])
        {
          ScAddr reusableComponent = searchComponentResult[i][COMPONENT_ALIAS];
          result.push_back(context->HelperGetSystemIdtf(reusableComponent));
        }
      }
    }

    return result;
  }
};
