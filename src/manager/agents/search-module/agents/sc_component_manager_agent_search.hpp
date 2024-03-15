#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "sc_component_manager_agent_search.generated.hpp"

namespace searchModule
{
class ScComponentManagerSearchAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  static bool CheckAction(ScMemoryContext & m_memoryCtx, ScAddr const & actionAddr, ScAddr const & actionAddrClass);
};
}  // namespace searchModule
