#pragma once

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "sc_component_manager_agent_search.generated.hpp"

namespace commandsModule
{
class ScComponentManagerSearchAgent : public ScAgent
{
  SC_CLASS(Agent, Event(scAgentsCommon::CoreKeynodes::question_initiated, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  bool checkAction(ScMemoryContext & m_memoryCtx, ScAddr const & actionAddr, ScAddr const & actionAddrClass);
  ScAddr getParameternodeUnderRelation(
      ScMemoryContext & m_memoryCtx,
      ScAddr const & actionAddr,
      ScAddr const & relation);
  std::map<std::string, ScAddr> getElementsOfSet(ScMemoryContext & m_memoryCtx, ScAddr const & set);
  static std::map<std::string, ScAddr> getElementsLinksOfSet(ScMemoryContext & m_memoryCtx, ScAddr const & set);
};
}  // namespace commandsModule
