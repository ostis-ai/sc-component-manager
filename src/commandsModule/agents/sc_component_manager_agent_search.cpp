#include "sc-agents-common/utils/CommonUtils.hpp"

#include "../../manager/commands/keynodes/ScComponentManagerKeynodes.hpp"
#include "manager/commands/command_search/sc_component_manager_command_search.hpp"

#include "keynodes/commands_keynodes.hpp"
#include "sc_component_manager_agent_search.hpp"

using namespace commandsModule;
using namespace keynodes;

SC_AGENT_IMPLEMENTATION(ScComponentManagerSearchAgent)
{
  ScAddr const & actionAddr = otherAddr;

  if (!checkAction(m_memoryCtx, actionAddr, commandsModule::CommandsKeynodes::action_components_search))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("ScComponentManagerSearchAgent started");
  keynodes::ScComponentManagerKeynodes::InitGlobal();

  std::map<std::string, std::vector<std::string>> commandParameters;

  ScAddr setOfAuthors =
      getParameternodeUnderRelation(m_memoryCtx, actionAddr, commandsModule::CommandsKeynodes::rrel_authors);
  std::map<std::string, ScAddr> authors = getElementsOfSet(m_memoryCtx, setOfAuthors);

  ScAddr setOfClasses =
      getParameternodeUnderRelation(m_memoryCtx, actionAddr, commandsModule::CommandsKeynodes::rrel_classes);
  std::map<std::string, ScAddr> classes = getElementsOfSet(m_memoryCtx, setOfClasses);

  ScAddr setOfExplanations =
      getParameternodeUnderRelation(m_memoryCtx, actionAddr, commandsModule::CommandsKeynodes::rrel_explanations);
  std::map<std::string, ScAddr> explanations = getElementsLinksOfSet(m_memoryCtx, setOfExplanations);

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

  ScComponentManagerCommandSearch command = ScComponentManagerCommandSearch();
  command.Execute(&m_memoryCtx, commandParameters);

  SC_LOG_DEBUG("ScComponentManagerSearchAgent finished");
  return SC_RESULT_OK;
};

bool ScComponentManagerSearchAgent::checkAction(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & actionAddr,
    ScAddr const & actionAddrClass)
{
  return m_memoryCtx.HelperCheckEdge(actionAddrClass, actionAddr, ScType::EdgeAccessConstPosPerm);
}

ScAddr ScComponentManagerSearchAgent::getParameternodeUnderRelation(
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

std::map<std::string, ScAddr> ScComponentManagerSearchAgent::getElementsOfSet(
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

std::map<std::string, ScAddr> ScComponentManagerSearchAgent::getElementsLinksOfSet(
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
