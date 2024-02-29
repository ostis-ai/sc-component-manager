#include "sc-agents-common/utils/CommonUtils.hpp"

#include "../../manager/commands/keynodes/ScComponentManagerKeynodes.hpp"
#include "manager/commands/command_install/sc_component_manager_command_install.hpp"

#include "keynodes/commands_keynodes.hpp"
#include "sc_component_manager_agent_install.hpp"

using namespace commandsModule;
using namespace keynodes;

SC_AGENT_IMPLEMENTATION(ScComponentManagerInstallAgent)
{
  ScAddr const & actionAddr = otherAddr;

  if (!checkAction(actionAddr))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("ScComponentManagerInstallAgent started");
  keynodes::ScComponentManagerKeynodes::InitGlobal();

  SC_LOG_DEBUG("ScComponentManagerInstallAgent finished");
  return SC_RESULT_OK;
};

bool ScComponentManagerInstallAgent::checkAction(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      commandsModule::CommandsKeynodes::action_components_install, actionAddr, ScType::EdgeAccessConstPosPerm);
}
