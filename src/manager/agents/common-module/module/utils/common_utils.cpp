/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "common_utils.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "sc-memory/sc_memory.hpp"

#include "constants/command_constants.hpp"
#include "module/keynodes/ScComponentManagerKeynodes.hpp"

namespace common_utils
{
std::map<std::string, ScAddr> CommonUtils::managerParametersWithAgentRelations;

void CommonUtils::InitParametersMap()
{
  managerParametersWithAgentRelations = {
      {"author", keynodes::ScComponentManagerKeynodes::rrel_author},
      {"class", keynodes::ScComponentManagerKeynodes::rrel_class},
      {"explanation", keynodes::ScComponentManagerKeynodes::rrel_explanation},
      {"idtf", scAgentsCommon::CoreKeynodes::rrel_1},
      {"search", keynodes::ScComponentManagerKeynodes::action_components_search},
      {"install", keynodes::ScComponentManagerKeynodes::action_components_install},
      {"init", keynodes::ScComponentManagerKeynodes::action_components_init}};
}

bool CommonUtils::TransformToScStruct(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & actionAddr,
    std::map<std::string, std::vector<std::string>> const & commandParameters)
{
  ScAddr parameterValueAddr;
  ScAddr parameterRrelNodeAddr;
  ScAddr edgeAddr;
  ScAddr setAddr;
  for (auto const & parameter : commandParameters)
  {
    try
    {
      parameterRrelNodeAddr = managerParametersWithAgentRelations.at(parameter.first);
    }
    catch (std::out_of_range const & exception)
    {
      SC_LOG_INFO("Transform to sc-structure: Unknown parameter " + parameter.first);
      continue;
    }
    setAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
    edgeAddr = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, actionAddr, setAddr);

    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, parameterRrelNodeAddr, edgeAddr);

    for (std::string const & parameterValue : parameter.second)
    {
      if (parameter.first == CommandsConstantsFlags::EXPLANATION)
      {
        parameterValueAddr = m_memoryCtx.CreateNode(ScType::LinkConst);
        m_memoryCtx.SetLinkContent(parameterValueAddr, parameterValue);
      }
      else
      {
        parameterValueAddr = m_memoryCtx.HelperFindBySystemIdtf(parameterValue);
        if (!parameterValueAddr.IsValid())
        {
          SC_LOG_WARNING("Transform to sc-structure: Unknown value: " + parameterValue);
          parameterValueAddr = m_memoryCtx.CreateNode(ScType::NodeConst);
          m_memoryCtx.HelperSetSystemIdtf(parameterValue, parameterValueAddr);
        }
      }
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, setAddr, parameterValueAddr);
    }
  }
  return true;
}

ScAddrVector CommonUtils::GetNodesUnderParameter(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & actionAddr,
    ScAddr const & relationAddr)
{
  ScAddr parameterNode;
  ScAddrVector components;
  ScIterator5Ptr const & parameterIterator = m_memoryCtx.Iterator5(
      actionAddr, ScType::EdgeAccessConstPosPerm, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, relationAddr);
  if (parameterIterator->Next())
  {
    parameterNode = parameterIterator->Get(2);
    ScIterator3Ptr const & componentsIterator =
        m_memoryCtx.Iterator3(parameterNode, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (componentsIterator->Next())
    {
      if (m_memoryCtx.GetElementType(componentsIterator->Get(2)) == ScType::NodeConstClass)
      {
        ScIterator3Ptr const & elementsIterator =
            m_memoryCtx.Iterator3(componentsIterator->Get(2), ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
        while (elementsIterator->Next())
        {
          components.push_back(elementsIterator->Get(2));
        }
      }
      else
      {
        components.push_back(componentsIterator->Get(2));
      }
    }
  }
  return components;
}

std::map<std::string, std::vector<std::string>> CommonUtils::GetCommandParameters(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & actionAddr)
{
  std::map<std::string, std::vector<std::string>> commandParameters;

  ScAddr const & authorsSetAddr =
      GetParameterNodeUnderRelation(m_memoryCtx, actionAddr, keynodes::ScComponentManagerKeynodes::rrel_author);
  std::map<std::string, ScAddr> const & authors = GetSetElements(m_memoryCtx, authorsSetAddr);

  ScAddr const & classesSetAddr =
      GetParameterNodeUnderRelation(m_memoryCtx, actionAddr, keynodes::ScComponentManagerKeynodes::rrel_class);
  std::map<std::string, ScAddr> const & classes = GetSetElements(m_memoryCtx, classesSetAddr);

  ScAddr const & explanationsSetAddr =
      GetParameterNodeUnderRelation(m_memoryCtx, actionAddr, keynodes::ScComponentManagerKeynodes::rrel_explanation);
  std::map<std::string, ScAddr> const & explanations = GetElementsLinksOfSet(m_memoryCtx, explanationsSetAddr);

  std::vector<std::string> authorsList, classesList, explanationsList;

  if (authorsSetAddr.IsValid())
  {
    for (auto & el : authors)
      authorsList.push_back(el.first);
    commandParameters.insert({CommandsConstantsFlags::AUTHOR, authorsList});
  }
  if (classesSetAddr.IsValid())
  {
    for (auto & el : classes)
      classesList.push_back(el.first);
    commandParameters.insert({CommandsConstantsFlags::CLASS, classesList});
  }
  if (explanationsSetAddr.IsValid())
  {
    for (auto & el : explanations)
      explanationsList.push_back(el.first);
    commandParameters.insert({CommandsConstantsFlags::EXPLANATION, explanationsList});
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

std::map<std::string, ScAddr> CommonUtils::GetSetElements(ScMemoryContext & m_memoryCtx, ScAddr const & setAddr)
{
  std::map<std::string, ScAddr> elementsIdtfAndAddr;
  if (!setAddr.IsValid())
    return elementsIdtfAndAddr;
  ScIterator3Ptr const & elementsIterator =
      m_memoryCtx.Iterator3(setAddr, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
  while (elementsIterator->Next())
  {
    try
    {
      elementsIdtfAndAddr.insert({m_memoryCtx.HelperGetSystemIdtf(elementsIterator->Get(2)), elementsIterator->Get(2)});
    }
    catch (std::exception const & exception)
    {
      SC_LOG_WARNING("CommonUtils::GetSetElements : met element without system identifier");
      continue;
    }
  }
  return elementsIdtfAndAddr;
}

std::map<std::string, ScAddr> CommonUtils::GetElementsLinksOfSet(ScMemoryContext & m_memoryCtx, ScAddr const & setAddr)
{
  std::map<std::string, ScAddr> elementsIdtfAndAddr;
  if (!setAddr.IsValid())
    return elementsIdtfAndAddr;
  std::string elementIdtf;
  ScIterator3Ptr const & elementsIterator =
      m_memoryCtx.Iterator3(setAddr, ScType::EdgeAccessConstPosPerm, ScType::LinkConst);
  while (elementsIterator->Next())
  {
    m_memoryCtx.GetLinkContent(elementsIterator->Get(2), elementIdtf);
    elementsIdtfAndAddr.insert({elementIdtf, elementsIterator->Get(2)});
  }
  return elementsIdtfAndAddr;
}

}  // namespace common_utils
