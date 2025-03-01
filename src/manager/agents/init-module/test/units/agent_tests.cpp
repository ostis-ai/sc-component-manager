/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_agents_test.hpp"

#include <common/sc_component_manager_keynodes.hpp>

#include "agents/sc_component_manager_agent_init.hpp"

TEST_F(ScAgentsTest, AgentInit)
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
