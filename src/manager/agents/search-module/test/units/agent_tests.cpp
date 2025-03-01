/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_agents_test.hpp"

#include <common/sc_component_manager_keynodes.hpp>

#include "agents/sc_component_manager_agent_search.hpp"

std::unordered_set<std::string> const ALL_SPECIFICATIONS_FOR_SEARCH = {
    "part_platform_specification",
    "part_ui_specification",
    "specification_1",
    "specification_2",
    "specification_3",
};

void searchComponentsTestBody(
    ScAgentContext & context,
    std::unordered_set<std::string> const & expectedFoundSpecifications,
    std::string const & filename)
{
  ScAgentsTest::loader.loadScsFile(context, ScAgentsTest::TEST_FILES_COMPONENTS_SEARCH_DIR_PATH + "specifications.scs");
  ScAgentsTest::loader.loadScsFile(context, ScAgentsTest::TEST_FILES_COMPONENTS_SEARCH_DIR_PATH + filename);
  ScAddr const & testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<searchModule::ScComponentManagerSearchAgent>();
  testAction.InitiateAndWait(ScAgentsTest::WAIT_TIME);
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

TEST_F(ScAgentsTest, AgentSearchByOneClass)
{
  searchComponentsTestBody(
      *m_ctx, {"part_platform_specification", "part_ui_specification"}, "one_class_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByTwoClasses)
{
  searchComponentsTestBody(
      *m_ctx, {"part_platform_specification", "part_ui_specification"}, "two_classes_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByZeroClasses)
{
  searchComponentsTestBody(*m_ctx, ALL_SPECIFICATIONS_FOR_SEARCH, "zero_classes_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByIncompatibleClasses)
{
  searchComponentsTestBody(*m_ctx, {}, "incompatible_classes_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByOneAuthor)
{
  searchComponentsTestBody(*m_ctx, {"specification_1", "specification_2"}, "one_author_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByTwoAuthors)
{
  searchComponentsTestBody(*m_ctx, {"specification_2"}, "two_authors_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByZeroAuthors)
{
  searchComponentsTestBody(
      *m_ctx, {"specification_1", "specification_2", "specification_3"}, "zero_authors_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByIncompatibleAuthor)
{
  searchComponentsTestBody(*m_ctx, {}, "incompatible_authors_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByOneKey)
{
  searchComponentsTestBody(*m_ctx, {"specification_3"}, "one_key_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByTwoKeys)
{
  searchComponentsTestBody(*m_ctx, {"specification_2"}, "two_keys_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByZeroKeys)
{
  searchComponentsTestBody(
      *m_ctx, {"specification_1", "specification_2", "specification_3"}, "zero_keys_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByIncompatibleKeys)
{
  searchComponentsTestBody(*m_ctx, {}, "incompatible_keys_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByOneNote)
{
  searchComponentsTestBody(*m_ctx, {"specification_2"}, "one_note_action_components_search.scs");
}

// disabled because links in action arguments are passed without order so search uses `2 component` instead of
// `component 2`
TEST_F(ScAgentsTest, DISABLED_AgentSearchByTwoNotes)
{
  searchComponentsTestBody(*m_ctx, {"specification_2"}, "two_notes_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByZeroNotes)
{
  searchComponentsTestBody(*m_ctx, ALL_SPECIFICATIONS_FOR_SEARCH, "zero_notes_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByIncompatibleNotes)
{
  searchComponentsTestBody(*m_ctx, {}, "incompatible_notes_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByOneExplanation)
{
  searchComponentsTestBody(
      *m_ctx, {"specification_1", "specification_3"}, "one_explanation_action_components_search.scs");
}

// disabled because links in action arguments are passed without order so search uses `2 component` instead of
// `component 2`
TEST_F(ScAgentsTest, DISABLED_AgentSearchByTwoExplanations)
{
  searchComponentsTestBody(*m_ctx, {"specification_2"}, "two_explanations_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByZeroExplanations)
{
  searchComponentsTestBody(*m_ctx, ALL_SPECIFICATIONS_FOR_SEARCH, "zero_explanations_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByIncompatibleExplanations)
{
  searchComponentsTestBody(*m_ctx, {}, "incompatible_explanations_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByOnePurpose)
{
  searchComponentsTestBody(*m_ctx, {"specification_1", "specification_2"}, "one_purpose_action_components_search.scs");
}

// disabled because links in action arguments are passed without order so search uses `2 component` instead of
// `component 2`
TEST_F(ScAgentsTest, DISABLED_AgentSearchByTwoPurposes)
{
  searchComponentsTestBody(*m_ctx, {"specification_2"}, "two_purposes_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByZeroPurposes)
{
  searchComponentsTestBody(*m_ctx, ALL_SPECIFICATIONS_FOR_SEARCH, "zero_purposes_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByIncompatiblePurposes)
{
  searchComponentsTestBody(*m_ctx, {}, "incompatible_purposes_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByOneId)
{
  searchComponentsTestBody(*m_ctx, {"specification_1", "specification_2"}, "one_id_action_components_search.scs");
}

// disabled because links in action arguments are passed without order so search uses `3 component` instead of
// `component 3`
TEST_F(ScAgentsTest, DISABLED_AgentSearchByTwoIds)
{
  searchComponentsTestBody(*m_ctx, {"specification_3"}, "two_ids_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByZeroIds)
{
  searchComponentsTestBody(*m_ctx, ALL_SPECIFICATIONS_FOR_SEARCH, "zero_ids_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByIncompatibleIds)
{
  searchComponentsTestBody(*m_ctx, {}, "incompatible_ids_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchByAllArguments)
{
  searchComponentsTestBody(*m_ctx, {"specification_2"}, "all_arguments_action_components_search.scs");
}

TEST_F(ScAgentsTest, AgentSearchWithEmptyArguments)
{
  searchComponentsTestBody(*m_ctx, ALL_SPECIFICATIONS_FOR_SEARCH, "empty_arguments_action_components_search.scs");
}
