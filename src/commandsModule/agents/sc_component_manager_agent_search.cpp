#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"

#include "manager/commands/keynodes/ScComponentManagerKeynodes.hpp"

#include "keynodes/commands_keynodes.hpp"
#include "sc_component_manager_agent_search.hpp"

using namespace commandsModule;
using namespace keynodes;

std::map<std::string, std::string> ScComponentManagerSearchAgent::managerParametersWithAgentRelations = {
    {"author", "rrel_author"},
    {"class", "rrel_class"},
    {"explanation", "rrel_explanation"}};

SC_AGENT_IMPLEMENTATION(ScComponentManagerSearchAgent)
{
  ScAddr const & actionAddr = otherAddr;

  if (!checkAction(m_memoryCtx, actionAddr, commandsModule::CommandsKeynodes::action_components_search))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("ScComponentManagerSearchAgent started");
  keynodes::ScComponentManagerKeynodes::InitGlobal();

  //  ScComponentManagerCommandSearch command = ScComponentManagerCommandSearch();
  //  command.ExecuteAddr(&m_memoryCtx, actionAddr);

  ScAddr parameterClassNode =
      getParameterNodeUnderRelation(m_memoryCtx, actionAddr, commandsModule::CommandsKeynodes::rrel_class);
  ScAddr parameterAuthorNode =
      getParameterNodeUnderRelation(m_memoryCtx, actionAddr, commandsModule::CommandsKeynodes::rrel_author);
  ScAddr parameterExplanationNode =
      getParameterNodeUnderRelation(m_memoryCtx, actionAddr, commandsModule::CommandsKeynodes::rrel_explanation);

  // allComponents stores reusable components that meet a condition
  std::vector<ScAddrVector> allComponents;

  if (parameterClassNode.IsValid())
  {
    std::map<std::string, ScAddr> parametersValuesClass = getElementsOfSet(m_memoryCtx, parameterClassNode);
    for (auto const & value : parametersValuesClass)
    {
      allComponents.push_back(getComponentsByClass(m_memoryCtx, value.second));
    }
  }
  if (parameterAuthorNode.IsValid())
  {
    std::map<std::string, ScAddr> parametersValuesAuthor = getElementsOfSet(m_memoryCtx, parameterAuthorNode);
    for (auto const & value : parametersValuesAuthor)
    {
      allComponents.push_back(getComponentsByAuthor(m_memoryCtx, value.second));
    }
  }
  if (parameterExplanationNode.IsValid())
  {
    std::map<std::string, ScAddr> parametersValuesExplanation =
        getElementsLinksOfSet(m_memoryCtx, parameterExplanationNode);
    for (auto const & value : parametersValuesExplanation)
    {
      allComponents.push_back(getComponentsByExplanation(m_memoryCtx, value.second));
    }
  }
  if (!parameterExplanationNode.IsValid() && !parameterClassNode.IsValid() && !parameterAuthorNode.IsValid())
  {
    allComponents.push_back(
        getComponentsByClass(m_memoryCtx, keynodes::ScComponentManagerKeynodes::concept_reusable_component));
  }
  ScAddrVector result;

  // next code intersects allComponents in one result-vector
  while (allComponents.size() > 1)
  {
    allComponents[1] = intersection(m_memoryCtx, allComponents[0], allComponents[1]);
    allComponents.erase(allComponents.begin());
  }

  // if exists a component/s that meets all conditions
  if (!allComponents.empty())
    result = allComponents[0];
  else
    return SC_RESULT_OK;

  ScAddr components = m_memoryCtx.CreateNode(ScType::NodeConst);

  ScAddrVector::iterator foundComponentsIterator = result.begin();
  for (; foundComponentsIterator != result.end(); foundComponentsIterator++)
  {
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, components, *foundComponentsIterator);
  }

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, result, true);

  SC_LOG_DEBUG("ScComponentManagerSearchAgent finished");
  return SC_RESULT_OK;
};

ScAddrVector ScComponentManagerSearchAgent::intersection(
    ScMemoryContext & m_memoryCtx,
    ScAddrVector first,
    ScAddrVector second)
{
  ScAddrVector result;
  for (size_t i = 0; i < first.size(); i++)
  {
    for (size_t j = 0; j < second.size(); j++)
    {
      if (first[i] == second[j] && std::find(result.begin(), result.end(), first[i]) == result.end())
        result.push_back(first[i]);
    }
  }
  return result;
}

ScAddrVector ScComponentManagerSearchAgent::getComponentsByExplanation(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & componentExplanation)
{
  ScAddrVector components;
  ScAddr component;
  ScIterator3Ptr const & componentsIterator = m_memoryCtx.Iterator3(
      keynodes::ScComponentManagerKeynodes::concept_reusable_component,
      ScType::EdgeAccessConstPosPerm,
      ScType::NodeConst);
  while (componentsIterator->Next())
  {
    component = componentsIterator->Get(2);
    ScIterator5Ptr const & explanationIterator = m_memoryCtx.Iterator5(
        component,
        ScType::EdgeDCommonConst,
        ScType::LinkConst,
        ScType::EdgeAccessConstPosPerm,
        keynodes::ScComponentManagerKeynodes::nrel_explanation);
    if (explanationIterator->Next())
    {
      std::string foundLink, givenLink;
      m_memoryCtx.GetLinkContent(explanationIterator->Get(2), foundLink);
      m_memoryCtx.GetLinkContent(componentExplanation, givenLink);
      if (foundLink.find(givenLink) != std::string::npos)
      {
        components.push_back(component);
      }
    }
  }
  return components;
}

ScAddrVector ScComponentManagerSearchAgent::getComponentsByAuthor(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & componentAuthor)
{
  ScAddrVector components;
  ScAddr component;
  ScIterator3Ptr const & componentsIterator = m_memoryCtx.Iterator3(
      keynodes::ScComponentManagerKeynodes::concept_reusable_component,
      ScType::EdgeAccessConstPosPerm,
      ScType::NodeConst);
  while (componentsIterator->Next())
  {
    component = componentsIterator->Get(2);
    ScIterator5Ptr const & authorIterator = m_memoryCtx.Iterator5(
        component,
        ScType::EdgeDCommonConst,
        ScType::NodeConst,
        ScType::EdgeAccessConstPosPerm,
        keynodes::ScComponentManagerKeynodes::nrel_authors);
    if (authorIterator->Next())
    {
      if (m_memoryCtx.HelperCheckEdge(authorIterator->Get(2), componentAuthor, ScType::EdgeAccessConstPosPerm))
      {
        components.push_back(component);
      }
    }
  }
  return components;
}

ScAddrVector ScComponentManagerSearchAgent::getComponentsByClass(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & componentClass)
{
  ScAddrVector components;
  ScAddr component;
  ScIterator3Ptr const & componentsIterator =
      m_memoryCtx.Iterator3(componentClass, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
  while (componentsIterator->Next())
  {
    component = componentsIterator->Get(2);
    if (m_memoryCtx.HelperCheckEdge(
            keynodes::ScComponentManagerKeynodes::concept_reusable_component,
            component,
            ScType::EdgeAccessConstPosPerm))
      components.push_back(component);
  }
  return components;
}

bool ScComponentManagerSearchAgent::checkAction(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & actionAddr,
    ScAddr const & actionAddrClass)
{
  return m_memoryCtx.HelperCheckEdge(actionAddrClass, actionAddr, ScType::EdgeAccessConstPosPerm);
}

std::map<std::string, std::vector<std::string>> ScComponentManagerSearchAgent::getCommandParameters(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & actionAddr)
{
  std::map<std::string, std::vector<std::string>> commandParameters;

  ScAddr setOfAuthors =
      getParameterNodeUnderRelation(m_memoryCtx, actionAddr, commandsModule::CommandsKeynodes::rrel_author);
  std::map<std::string, ScAddr> authors = getElementsOfSet(m_memoryCtx, setOfAuthors);

  ScAddr setOfClasses =
      getParameterNodeUnderRelation(m_memoryCtx, actionAddr, commandsModule::CommandsKeynodes::rrel_class);
  std::map<std::string, ScAddr> classes = getElementsOfSet(m_memoryCtx, setOfClasses);

  ScAddr setOfExplanations =
      getParameterNodeUnderRelation(m_memoryCtx, actionAddr, commandsModule::CommandsKeynodes::rrel_explanation);
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
  return commandParameters;
}

ScAddr ScComponentManagerSearchAgent::getParameterNodeUnderRelation(
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

// example: actionAddr : RR ->_(edge) set (-> value1, value2)
bool ScComponentManagerSearchAgent::TransformToScStruct(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & actionAddr,
    std::map<std::string, std::vector<std::string>> const & commandParameters)
{
  ScAddr value, parameterRRelNode, edge, set;
  for (const auto & parameter : commandParameters)
  {
    parameterRRelNode = m_memoryCtx.HelperFindBySystemIdtf(managerParametersWithAgentRelations.at(parameter.first));

    set = m_memoryCtx.CreateNode(ScType::NodeConst);
    edge = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, actionAddr, set);

    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, parameterRRelNode, edge);

    for (const std::string & valueOfParameter : parameter.second)
    {
      value = m_memoryCtx.CreateNode(ScType::NodeConst);
      m_memoryCtx.HelperSetSystemIdtf(valueOfParameter, value);
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, set, value);
    }
  }
  return true;
}
