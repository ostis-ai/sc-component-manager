#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "sc_component_manager_agent_install.generated.hpp"

namespace commandsModule
{
class ScComponentManagerInstallAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  bool checkAction(ScAddr const & actionAddr);
  static ScAddr getParameternodeUnderRelation(
      ScMemoryContext & m_memoryCtx,
      ScAddr const & actionAddr,
      ScAddr const & relation);
};
}  // namespace commandsModule
