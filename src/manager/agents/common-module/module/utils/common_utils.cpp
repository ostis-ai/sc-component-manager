/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "common_utils.hpp"

#include "sc-memory/sc_memory.hpp"
#include "module/keynodes/ScComponentManagerKeynodes.hpp"

namespace common_utils
{
std::map<std::string, ScAddr> CommonUtils::managerParametersWithAgentRelations = {
    {"author", keynodes::ScComponentManagerKeynodes::rrel_author},
    {"class", keynodes::ScComponentManagerKeynodes::rrel_class},
    {"explanation", keynodes::ScComponentManagerKeynodes::rrel_explanation},
    {"idtf", keynodes::ScComponentManagerKeynodes::rrel_identifier},
    {"search", keynodes::ScComponentManagerKeynodes::action_components_search},
    {"install", keynodes::ScComponentManagerKeynodes::action_components_install},
    {"init", keynodes::ScComponentManagerKeynodes::action_components_init}};

bool CommonUtils::TransformToScStruct(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & actionAddr,
    std::map<std::string, std::vector<std::string>> const & commandParameters)
{
  ScAddr value;
  ScAddr parameterRRelNode;
  ScAddr edge;
  ScAddr set;
  for (const auto & parameter : commandParameters)
  {
    try
    {
      parameterRRelNode = managerParametersWithAgentRelations.at(parameter.first);
    }
    catch (std::out_of_range const & exception)
    {
      SC_LOG_INFO("Transform to sc-structure: Unknown parameter " + parameter.first);
      continue;
    }
    set = m_memoryCtx.CreateNode(ScType::NodeConst);
    edge = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, actionAddr, set);

    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, parameterRRelNode, edge);

    for (std::string const & valueOfParameter : parameter.second)
    {
      if (parameter.first == "explanation")
      {
        value = m_memoryCtx.CreateNode(ScType::LinkConst);
        m_memoryCtx.SetLinkContent(value, valueOfParameter);
      }
      else
      {
        value = m_memoryCtx.HelperFindBySystemIdtf(valueOfParameter);
        if (!value.IsValid())
          continue;
      }
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, set, value);
    }
  }
  return true;
}

ScAddrVector CommonUtils::GetNodesUnderParameter(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & actionAddr,
    ScAddr const & relation)
{
  ScAddr parameterNode;
  ScAddrVector components;
  ScIterator5Ptr const & parameterIterator = m_memoryCtx.Iterator5(
      actionAddr, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, relation);
  if (parameterIterator->Next())
  {
    parameterNode = parameterIterator->Get(2);
    ScIterator3Ptr const & componentsIterator =
        m_memoryCtx.Iterator3(parameterNode, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (componentsIterator->Next())
    {
      components.push_back(componentsIterator->Get(2));
    }
  }
  return components;
}

std::map<std::string, std::vector<std::string>> CommonUtils::GetCommandParameters(
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

ScAddr CommonUtils::GetParameterNodeUnderRelation(
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

std::map<std::string, ScAddr> CommonUtils::GetElementsOfSet(ScMemoryContext & m_memoryCtx, ScAddr const & set)
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

std::map<std::string, ScAddr> CommonUtils::GetElementsLinksOfSet(ScMemoryContext & m_memoryCtx, ScAddr const & set)
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

}  // namespace common_utils
