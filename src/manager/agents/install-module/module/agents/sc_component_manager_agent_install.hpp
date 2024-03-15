#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "sc_component_manager_agent_install.generated.hpp"

namespace installModule
{
class ScComponentManagerInstallAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  bool CheckAction(ScAddr const & actionAddr);
};
}  // namespace installModule
