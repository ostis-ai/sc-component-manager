/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>
#include <sc-builder/src/scs_loader.hpp>
#include "sc_test.hpp"

#include "common-module/module/keynodes/ScComponentManagerKeynodes.hpp"

#include "init-module/module/agents/sc_component_manager_agent_init.hpp"
#include "search-module/module/agents/sc_component_manager_agent_search.hpp"
#include "install-module/module/agents/sc_component_manager_agent_install.hpp"

using AgentTest = ScMemoryTest;
ScsLoader loader;
const std::string TEST_FILES_DIR_PATH = MODULE_TEST_SRC_PATH "/testStructures/";

int const WAIT_TIME = 5000;

TEST_F(AgentTest, AgentInit)
{
  ScAgentContext & context = *m_ctx;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "action_components_init.scs");
  ScAddr const & testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
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

TEST_F(AgentTest, AgentSearch)
{
  ScAgentContext & context = *m_ctx;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "action_components_search.scs");
  ScAddr const & testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<searchModule::ScComponentManagerSearchAgent>();
  std::vector<std::string> namesOfSpecifications = {"part_platform_specification", "part_ui_specification"};
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
    isSpecificationExists =
        std::count(namesOfSpecifications.begin(), namesOfSpecifications.end(), specificationIdentifier);
    EXPECT_TRUE(isSpecificationExists) << specificationIdentifier << " is not in action result";
    foundSpecifications += isSpecificationExists;
  }
  EXPECT_EQ(foundSpecifications, namesOfSpecifications.size());
  context.UnsubscribeAgent<searchModule::ScComponentManagerSearchAgent>();
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
