/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_agents_test.hpp"

#include <common/sc_component_manager_keynodes.hpp>

#include "agents/sc_component_manager_agent_install.hpp"

TEST_F(ScAgentsTest, AgentInstall)
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
