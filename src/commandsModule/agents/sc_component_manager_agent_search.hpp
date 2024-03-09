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

  static ScAddrVector intersection(ScMemoryContext & m_memoryCtx, ScAddrVector first, ScAddrVector second);
  static ScAddrVector getComponentsByExplanation(ScMemoryContext & m_memoryCtx, ScAddr const & componentExplanation);
  static ScAddrVector getComponentsByAuthor(ScMemoryContext & m_memoryCtx, ScAddr const & componentAuthor);
  static ScAddrVector getComponentsByClass(ScMemoryContext & m_memoryCtx, ScAddr const & componentClass);
  static bool TransformToScStruct(
      ScMemoryContext & m_memoryCtx,
      ScAddr const & actionAddr,
      std::map<std::string, std::vector<std::string>> const & commandParameters);
  static ScAddr getParameterNodeUnderRelation(
      ScMemoryContext & m_memoryCtx,
      ScAddr const & actionAddr,
      ScAddr const & relation);
  static std::map<std::string, ScAddr> getElementsOfSet(ScMemoryContext & m_memoryCtx, ScAddr const & set);
  static std::map<std::string, ScAddr> getElementsLinksOfSet(ScMemoryContext & m_memoryCtx, ScAddr const & set);
  static std::map<std::string, std::vector<std::string>> getCommandParameters(
      ScMemoryContext & m_memoryCtx,
      ScAddr const & actionAddr);

private:
  static std::map<std::string, std::string> managerParametersWithAgentRelations;
  static bool checkAction(ScMemoryContext & m_memoryCtx, ScAddr const & actionAddr, ScAddr const & actionAddrClass);
};
}  // namespace commandsModule
