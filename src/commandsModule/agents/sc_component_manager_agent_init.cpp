#include "sc-agents-common/utils/CommonUtils.hpp"

#include "../../manager/commands/keynodes/ScComponentManagerKeynodes.hpp"
#include "manager/commands/command_init/sc_component_manager_command_init.hpp"

#include "keynodes/commands_keynodes.hpp"
#include "sc_component_manager_agent_init.hpp"

using namespace commandsModule;
using namespace keynodes;

SC_AGENT_IMPLEMENTATION(ScComponentManagerInitAgent)
{
  ScAddr const & actionAddr = otherAddr;

  if (!checkAction(actionAddr))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("ScComponentManagerInitAgent started");
  keynodes::ScComponentManagerKeynodes::InitGlobal();
  // TODO: change config_path
  std::string const config_path = "/home/anna/projects/ostis-metasystem/sc-models/knowledge-base";
  ScComponentManagerCommandInit command = ScComponentManagerCommandInit(config_path);
  SC_LOG_ERROR(config_path);
  command.Execute(&m_memoryCtx, {});
  SC_LOG_DEBUG("ScComponentManagerInitAgent finished");
  return SC_RESULT_OK;
};

bool ScComponentManagerInitAgent::checkAction(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      commandsModule::CommandsKeynodes::action_components_init, actionAddr, ScType::EdgeAccessConstPosPerm);
}
