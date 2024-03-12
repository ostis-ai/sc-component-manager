#include <sc-agents-common/utils/AgentUtils.hpp>
#include "keynodes/ScComponentManagerKeynodes.hpp"
#include "utils/sc_component_manager_command_search.hpp"

#include "sc_component_manager_agent_search.hpp"

using namespace searchModule;
using namespace keynodes;

SC_AGENT_IMPLEMENTATION(ScComponentManagerSearchAgent)
{
  ScAddr const & actionAddr = otherAddr;

  keynodes::ScComponentManagerKeynodes::InitGlobal();
  if (!CheckAction(m_memoryCtx, actionAddr, keynodes::ScComponentManagerKeynodes::action_components_search))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("ScComponentManagerSearchAgent started");
  keynodes::ScComponentManagerKeynodes::InitGlobal();

  ScComponentManagerCommandSearch command = ScComponentManagerCommandSearch();
  ScAddrVector components = command.Execute(&m_memoryCtx, actionAddr);

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, components, true);

  SC_LOG_DEBUG("ScComponentManagerSearchAgent finished");
  return SC_RESULT_OK;
}

bool ScComponentManagerSearchAgent::CheckAction(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & actionAddr,
    ScAddr const & actionAddrClass)
{
  return m_memoryCtx.HelperCheckEdge(actionAddrClass, actionAddr, ScType::EdgeAccessConstPosPerm);
}

std::map<std::string, std::vector<std::string>> ScComponentManagerSearchAgent::GetCommandParameters(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & actionAddr)
{
  std::map<std::string, std::vector<std::string>> commandParameters;

  ScAddr setOfAuthors =
      GetParameterNodeUnderRelation(m_memoryCtx, actionAddr, keynodes::ScComponentManagerKeynodes::rrel_author);
  std::map<std::string, ScAddr> authors = GetElementsOfSet(m_memoryCtx, setOfAuthors);

  ScAddr setOfClasses =
      GetParameterNodeUnderRelation(m_memoryCtx, actionAddr, keynodes::ScComponentManagerKeynodes::rrel_class);
  std::map<std::string, ScAddr> classes = GetElementsOfSet(m_memoryCtx, setOfClasses);

  ScAddr setOfExplanations =
      GetParameterNodeUnderRelation(m_memoryCtx, actionAddr, keynodes::ScComponentManagerKeynodes::rrel_explanation);
  std::map<std::string, ScAddr> explanations = GetElementsLinksOfSet(m_memoryCtx, setOfExplanations);

  std::vector<std::string> authorsList, classesList, explanationsList;

  if (setOfAuthors.IsValid())
  {
    for (auto & el : authors)
      authorsList.push_back(el.first);
    commandParameters.insert({"author", authorsList});
  }
  if (setOfClasses.IsValid())
  {
    for (auto & el : classes)
      classesList.push_back(el.first);
    commandParameters.insert({"class", classesList});
  }
  if (setOfExplanations.IsValid())
  {
    for (auto & el : explanations)
      explanationsList.push_back(el.first);
    commandParameters.insert({"explanation", explanationsList});
  }
  return commandParameters;
}

ScAddr ScComponentManagerSearchAgent::GetParameterNodeUnderRelation(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & actionAddr,
    ScAddr const & relation)
{
  ScAddr parameterNode;
  ScIterator5Ptr const & parameterIterator = m_memoryCtx.Iterator5(
      actionAddr, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, relation);
  if (parameterIterator->Next())
  {
    parameterNode = parameterIterator->Get(2);
  }
  return parameterNode;
}

std::map<std::string, ScAddr> ScComponentManagerSearchAgent::GetElementsOfSet(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & set)
{
  std::map<std::string, ScAddr> elements;
  ScIterator3Ptr const & elementsIterator =
      m_memoryCtx.Iterator3(set, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
  while (elementsIterator->Next())
  {
    elements.insert({m_memoryCtx.HelperGetSystemIdtf(elementsIterator->Get(2)), elementsIterator->Get(2)});
  }
  return elements;
}

std::map<std::string, ScAddr> ScComponentManagerSearchAgent::GetElementsLinksOfSet(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & set)
{
  std::map<std::string, ScAddr> elements;
  std::string elementIdtf;
  ScIterator3Ptr const & elementsIterator =
      m_memoryCtx.Iterator3(set, ScType::EdgeAccessConstPosPerm, ScType::LinkConst);
  while (elementsIterator->Next())
  {
    m_memoryCtx.GetLinkContent(elementsIterator->Get(2), elementIdtf);
    elements.insert({elementIdtf, elementsIterator->Get(2)});
  }
  return elements;
}
