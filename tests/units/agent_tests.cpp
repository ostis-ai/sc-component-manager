/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>
#include <sc-builder/scs_loader.hpp>
#include <sc-memory/test/sc_test.hpp>

#include "common-module/module/keynodes/ScComponentManagerKeynodes.hpp"

#include "init-module/module/agents/sc_component_manager_agent_init.hpp"
#include "search-module/module/agents/sc_component_manager_agent_search.hpp"
#include "install-module/module/agents/sc_component_manager_agent_install.hpp"

using AgentTest = ScMemoryTest;
ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = MODULE_TEST_SRC_PATH "/testStructures/";
std::string const TEST_FILES_COMPONENTS_SEARCH_DIR_PATH = MODULE_TEST_SRC_PATH "/testStructures/componentsSearch/";

std::unordered_set<std::string> const ALL_SPECIFICATIONS_FOR_SEARCH = {
    "part_platform_specification",
    "part_ui_specification",
    "specification_1",
    "specification_2",
    "specification_3",
};

int const WAIT_TIME = 5000;

TEST_F(AgentTest, AgentInit)
{
  ScAgentContext & context = *m_ctx;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "action_components_init.scs");
  ScAddr const & testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(testActionNode.IsValid());
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<initModule::ScComponentManagerInitAgent>();
  testAction.InitiateAndWait(WAIT_TIME);
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());
  ScStructure result = testAction.GetResult();

  std::vector<std::string> namesOfSpecifications = {"part_platform_spec", "part_ui_spec"};
  bool isSpecificationExists = false;
  size_t foundSpecifications = 0;

  ScIterator3Ptr const & specificationsIterator =
      context.CreateIterator3(result, ScType::ConstPermPosArc, ScType::ConstNodeStructure);
  while (specificationsIterator->Next())
  {
    std::string const & specificationIdentifier = context.GetElementSystemIdentifier(specificationsIterator->Get(2));
    isSpecificationExists =
        std::count(namesOfSpecifications.begin(), namesOfSpecifications.end(), specificationIdentifier);
    EXPECT_TRUE(isSpecificationExists) << specificationIdentifier << " is not in action result";
    foundSpecifications += isSpecificationExists;
  }
  EXPECT_EQ(foundSpecifications, namesOfSpecifications.size());
  context.UnsubscribeAgent<initModule::ScComponentManagerInitAgent>();
}

void searchComponentsTestBody(
    ScAgentContext & context,
    std::unordered_set<std::string> const & expectedFoundSpecifications,
    std::string const & filename)
{
  loader.loadScsFile(context, TEST_FILES_COMPONENTS_SEARCH_DIR_PATH + "specifications.scs");
  loader.loadScsFile(context, TEST_FILES_COMPONENTS_SEARCH_DIR_PATH + filename);
  ScAddr const & testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<searchModule::ScComponentManagerSearchAgent>();
  testAction.InitiateAndWait(WAIT_TIME);
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  ScStructure result = testAction.GetResult();
  bool isSpecificationExists = false;
  size_t foundSpecifications = 0;

  ScIterator3Ptr const & specificationsIterator =
      context.CreateIterator3(result, ScType::ConstPermPosArc, ScType::ConstNodeStructure);
  while (specificationsIterator->Next())
  {
    std::string const & specificationIdentifier = context.GetElementSystemIdentifier(specificationsIterator->Get(2));
    isSpecificationExists = expectedFoundSpecifications.count(specificationIdentifier);
    EXPECT_TRUE(isSpecificationExists) << specificationIdentifier << " is not in action result";
    foundSpecifications += isSpecificationExists;
  }
  EXPECT_EQ(foundSpecifications, expectedFoundSpecifications.size());
  context.UnsubscribeAgent<searchModule::ScComponentManagerSearchAgent>();
}

TEST_F(AgentTest, AgentSearchByOneClass)
{
  searchComponentsTestBody(
      *m_ctx, {"part_platform_specification", "part_ui_specification"}, "one_class_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByTwoClasses)
{
  searchComponentsTestBody(
      *m_ctx, {"part_platform_specification", "part_ui_specification"}, "two_classes_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByZeroClasses)
{
  searchComponentsTestBody(*m_ctx, ALL_SPECIFICATIONS_FOR_SEARCH, "zero_classes_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByIncompatibleClasses)
{
  searchComponentsTestBody(*m_ctx, {}, "incompatible_classes_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByOneAuthor)
{
  searchComponentsTestBody(*m_ctx, {"specification_1", "specification_2"}, "one_author_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByTwoAuthors)
{
  searchComponentsTestBody(*m_ctx, {"specification_2"}, "two_authors_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByZeroAuthors)
{
  searchComponentsTestBody(
      *m_ctx, {"specification_1", "specification_2", "specification_3"}, "zero_authors_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByIncompatibleAuthor)
{
  searchComponentsTestBody(*m_ctx, {}, "incompatible_authors_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByOneKey)
{
  searchComponentsTestBody(*m_ctx, {"specification_3"}, "one_key_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByTwoKeys)
{
  searchComponentsTestBody(*m_ctx, {"specification_2"}, "two_keys_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByZeroKeys)
{
  searchComponentsTestBody(
      *m_ctx, {"specification_1", "specification_2", "specification_3"}, "zero_keys_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByIncompatibleKeys)
{
  searchComponentsTestBody(*m_ctx, {}, "incompatible_keys_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByOneNote)
{
  searchComponentsTestBody(*m_ctx, {"specification_2"}, "one_note_action_components_search.scs");
}

// disabled because links in action arguments are passed without order so search uses `2 component` instead of
// `component 2`
TEST_F(AgentTest, DISABLED_AgentSearchByTwoNotes)
{
  searchComponentsTestBody(*m_ctx, {"specification_2"}, "two_notes_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByZeroNotes)
{
  searchComponentsTestBody(*m_ctx, ALL_SPECIFICATIONS_FOR_SEARCH, "zero_notes_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByIncompatibleNotes)
{
  searchComponentsTestBody(*m_ctx, {}, "incompatible_notes_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByOneExplanation)
{
  searchComponentsTestBody(
      *m_ctx, {"specification_1", "specification_3"}, "one_explanation_action_components_search.scs");
}

// disabled because links in action arguments are passed without order so search uses `2 component` instead of
// `component 2`
TEST_F(AgentTest, DISABLED_AgentSearchByTwoExplanations)
{
  searchComponentsTestBody(*m_ctx, {"specification_2"}, "two_explanations_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByZeroExplanations)
{
  searchComponentsTestBody(*m_ctx, ALL_SPECIFICATIONS_FOR_SEARCH, "zero_explanations_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByIncompatibleExplanations)
{
  searchComponentsTestBody(*m_ctx, {}, "incompatible_explanations_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByOnePurpose)
{
  searchComponentsTestBody(*m_ctx, {"specification_1", "specification_2"}, "one_purpose_action_components_search.scs");
}

// disabled because links in action arguments are passed without order so search uses `2 component` instead of
// `component 2`
TEST_F(AgentTest, DISABLED_AgentSearchByTwoPurposes)
{
  searchComponentsTestBody(*m_ctx, {"specification_2"}, "two_purposes_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByZeroPurposes)
{
  searchComponentsTestBody(*m_ctx, ALL_SPECIFICATIONS_FOR_SEARCH, "zero_purposes_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByIncompatiblePurposes)
{
  searchComponentsTestBody(*m_ctx, {}, "incompatible_purposes_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByOneId)
{
  searchComponentsTestBody(*m_ctx, {"specification_1", "specification_2"}, "one_id_action_components_search.scs");
}

// disabled because links in action arguments are passed without order so search uses `3 component` instead of
// `component 3`
TEST_F(AgentTest, DISABLED_AgentSearchByTwoIds)
{
  searchComponentsTestBody(*m_ctx, {"specification_3"}, "two_ids_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByZeroIds)
{
  searchComponentsTestBody(*m_ctx, ALL_SPECIFICATIONS_FOR_SEARCH, "zero_ids_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByIncompatibleIds)
{
  searchComponentsTestBody(*m_ctx, {}, "incompatible_ids_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchByAllArguments)
{
  searchComponentsTestBody(*m_ctx, {"specification_2"}, "all_arguments_action_components_search.scs");
}

TEST_F(AgentTest, AgentSearchWithEmptyArguments)
{
  searchComponentsTestBody(*m_ctx, ALL_SPECIFICATIONS_FOR_SEARCH, "empty_arguments_action_components_search.scs");
}

TEST_F(AgentTest, AgentInstall)
{
  ScAgentContext & context = *m_ctx;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "action_components_install.scs");
  ScAddr const & testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<installModule::ScComponentManagerInstallAgent>();

  testAction.InitiateAndWait(WAIT_TIME);
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());
  ScStructure result = testAction.GetResult();

  ScIterator3Ptr const & componentsIterator =
      context.CreateIterator3(result, ScType::ConstPermPosArc, ScType::ConstNode);
  EXPECT_TRUE(componentsIterator->Next());

  EXPECT_EQ("part_ui", context.GetElementSystemIdentifier(componentsIterator->Get(2)));

  context.UnsubscribeAgent<installModule::ScComponentManagerInstallAgent>();
}
