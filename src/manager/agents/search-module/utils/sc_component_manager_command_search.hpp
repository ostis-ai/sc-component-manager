/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "commands/sc_component_manager_command.hpp"

#include "module/utils/common_utils.hpp"

class ScComponentManagerCommandSearch : public ScComponentManagerCommand
{
public:
  ScComponentManagerCommandSearch() = default;

  common_utils::CommonUtils::ScAddrUnorderedSet Execute(ScMemoryContext * context, ScAddr const & actionAddr) override;

protected:
  std::string const SPECIFICATION_ALIAS = "_specification";
  std::string const COMPONENT_ALIAS = "_component";
  std::string const AUTHORS_SET_ALIAS = "_authors";
  std::string const EXPLANATION_LINK_ALIAS = "_explanation";

  std::string const AUTHOR = "author";
  std::string const CLASS = "class";
  std::string const EXPLANATION = "explanation";
  std::set<std::string> const possibleSearchParameters = {AUTHOR, CLASS, EXPLANATION};  // extend to more params

  void SearchComponentsByClass(
      ScMemoryContext * context,
      ScTemplate & searchComponentTemplate,
      std::vector<std::string> const & parameters);

  void SearchComponentsByRelationSet(
      ScMemoryContext * context,
      ScAddr const & relationAddr,
      std::string const & setAlias,
      ScTemplate & searchComponentTemplate,
      std::vector<std::string> const & parameters);

  common_utils::CommonUtils::ScAddrUnorderedSet SearchComponentsByRelationLink(
      ScMemoryContext * context,
      ScAddr const & relationAddr,
      std::string const & linkAlias,
      ScTemplate & searchComponentTemplate,
      std::vector<std::string> const & parameters);

  common_utils::CommonUtils::ScAddrUnorderedSet SearchComponentsSpecifications(
      ScMemoryContext * context,
      ScTemplate & searchComponentTemplate,
      std::map<std::string, common_utils::CommonUtils::ScAddrUnorderedSet> const & linksValues);

  common_utils::CommonUtils::ScAddrUnorderedSet SearchComponentsSpecificationsWithoutLinks(
      ScMemoryContext * context,
      ScTemplateSearchResult const & searchComponentResult);

  common_utils::CommonUtils::ScAddrUnorderedSet SearchComponentsSpecificationsWithLinks(
      ScMemoryContext * context,
      ScTemplateSearchResult const & searchComponentResult,
      std::map<std::string, common_utils::CommonUtils::ScAddrUnorderedSet> const & linksValues);
};
