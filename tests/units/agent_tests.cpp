/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>
#include <sc-builder/src/scs_loader.hpp>
#include <sc-agents-common/keynodes/coreKeynodes.hpp>
#include "sc_test.hpp"

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"

#include "../manager/commands/command_init/sc_component_manager_agent_init.hpp"
#include "../manager/commands/command_search/sc_component_manager_agent_search.hpp"
#include "../manager/commands/command_install/sc_component_manager_agent_install.hpp"
#include "../manager/commands/keynodes/ScComponentManagerKeynodes.hpp"

using AgentTest = ScMemoryTest;
ScsLoader loader;
const std::string TEST_FILES_DIR_PATH = MODULE_TEST_SRC_PATH "/testStructures/";

const int WAIT_TIME = 10000;

void initialize()
{
  scAgentsCommon::CoreKeynodes::InitGlobal();
  keynodes::ScComponentManagerKeynodes::InitGlobal();
}

TEST_F(AgentTest, AgentInit)
{
  ScMemoryContext & context = *m_ctx;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "action_components_init.scs");
  loader.loadScsFile(context, MODULE_KB_PATH "specifications.scs");
  ScAddr const & testActionNode = context.HelperFindBySystemIdtf("test_action_node");

  ScAgentInit(true);
  initialize();

  SC_AGENT_REGISTER(commandsModule::ScComponentManagerInitAgent)

  context.CreateEdge(ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::question_initiated, testActionNode);

  utils::AgentUtils::applyAction(&context, testActionNode, WAIT_TIME);
  EXPECT_TRUE(context.HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_successfully, testActionNode, ScType::EdgeAccessConstPosPerm));
  std::vector<std::string> namesOfComponents = {"part_platform", "part_ui", "part_methods_tools", "concept_cat"};
  ScAddrVector components;
  ScIterator3Ptr const & componentsIterator = context.Iterator3(
      keynodes::ScComponentManagerKeynodes::concept_reusable_component,
      ScType::EdgeAccessConstPosPerm,
      ScType::NodeConst);
  bool isComponentExists;
  while (componentsIterator->Next())
  {
    isComponentExists = std::count(
        namesOfComponents.begin(), namesOfComponents.end(), context.HelperGetSystemIdtf(componentsIterator->Get(2)));
    EXPECT_TRUE(isComponentExists);
  }

  SC_AGENT_UNREGISTER(commandsModule::ScComponentManagerInitAgent)
}

TEST_F(AgentTest, AgentSearch)
{
  ScMemoryContext & context = *m_ctx;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "action_components_search.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "specification.scs");
  ScAddr const & testActionNode = context.HelperFindBySystemIdtf("test_action_node");

  ScAgentInit(true);
  initialize();

  SC_AGENT_REGISTER(commandsModule::ScComponentManagerSearchAgent)

  context.CreateEdge(ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::question_initiated, testActionNode);

  ScAddr result = utils::AgentUtils::applyActionAndGetResultIfExists(&context, testActionNode, WAIT_TIME);

  EXPECT_TRUE(context.HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_successfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  ScIterator3Ptr const & componentsIterator =
      context.Iterator3(result, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
  if (componentsIterator->Next())
  {
    EXPECT_EQ("sc_web", context.HelperGetSystemIdtf(componentsIterator->Get(2)));
  }
  else
  {
    EXPECT_TRUE(false);
  }
  SC_AGENT_UNREGISTER(commandsModule::ScComponentManagerSearchAgent)
}

TEST_F(AgentTest, AgentInstall)
{
  ScMemoryContext & context = *m_ctx;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "action_components_install.scs");
  loader.loadScsFile(context, MODULE_KB_PATH "specifications.scs");
  ScAddr const & testActionNode = context.HelperFindBySystemIdtf("test_action_node");

  ScAgentInit(true);
  initialize();

  SC_AGENT_REGISTER(commandsModule::ScComponentManagerInstallAgent)

  context.CreateEdge(ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::question_initiated, testActionNode);

  ScAddr result = utils::AgentUtils::applyActionAndGetResultIfExists(&context, testActionNode, WAIT_TIME);

  EXPECT_TRUE(context.HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_successfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  ScIterator3Ptr const & componentsIterator =
      context.Iterator3(result, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
  if (componentsIterator->Next())
  {
    EXPECT_EQ("part_ui", context.HelperGetSystemIdtf(componentsIterator->Get(2)));
  }
  else
  {
    EXPECT_TRUE(false);
  }

  SC_AGENT_UNREGISTER(commandsModule::ScComponentManagerInstallAgent)
}
