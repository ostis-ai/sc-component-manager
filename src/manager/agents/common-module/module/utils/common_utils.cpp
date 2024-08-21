/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "common_utils.hpp"

#include <sc-agents-common/utils/GenerationUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/CommonUtils.hpp>
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "sc-memory/sc_memory.hpp"

#include "constants/command_constants.hpp"
#include "module/keynodes/ScComponentManagerKeynodes.hpp"

namespace common_utils
{

std::map<std::string, ScAddr> CommonUtils::managerParametersWithAgentRelations;
std::map<std::string, std::vector<ScAddr>> CommonUtils::mainFlagWithSubFlags;
std::vector<std::vector<ScAddr>> CommonUtils::componentsClasses;

ScAddr CommonUtils::GetMyselfDecompositionAddr(ScMemoryContext & context)
{
  return utils::IteratorUtils::getAnyByOutRelation(
      &context,
      keynodes::ScComponentManagerKeynodes::myself,
      keynodes::ScComponentManagerKeynodes::nrel_ostis_system_decomposition);
}

void CommonUtils::CreateMyselfDecomposition(ScMemoryContext & context)
{
  ScAddr myselfDecompositionAddr = context.CreateNode(ScType::NodeConst);
  utils::GenerationUtils::generateRelationBetween(
      &context,
      keynodes::ScComponentManagerKeynodes::myself,
      myselfDecompositionAddr,
      keynodes::ScComponentManagerKeynodes::nrel_ostis_system_decomposition);
  ScAddr componentAddr;
  ScAddr componentClassAddr;
  ScAddr componentDecompositionAddr;

  for (ScAddrVector const & subsystemAndComponentClass : componentsClasses)
  {
    componentClassAddr = subsystemAndComponentClass[1];
    componentAddr = context.CreateNode(ScType::NodeConst);

    context.CreateEdge(ScType::EdgeAccessConstPosPerm, componentClassAddr, componentAddr);
    context.CreateEdge(ScType::EdgeAccessConstPosPerm, myselfDecompositionAddr, componentAddr);

    componentDecompositionAddr = context.CreateNode(ScType::NodeConst);
    utils::GenerationUtils::generateRelationBetween(
        &context, componentAddr, componentDecompositionAddr, keynodes::ScComponentManagerKeynodes::nrel_decomposition);
  }
}

void CommonUtils::InitParametersMap()
{
  managerParametersWithAgentRelations = {
      {CommandsSearchFlags::KEY, scAgentsCommon::CoreKeynodes::rrel_key_sc_element},
      {CommandsSearchFlags::IDTF, keynodes::ScComponentManagerKeynodes::rrel_components},
      {CommandsSearchFlags::SET, keynodes::ScComponentManagerKeynodes::rrel_sets}};

  mainFlagWithSubFlags = {
      {"rrel_1",
       {keynodes::ScComponentManagerKeynodes::rrel_components, keynodes::ScComponentManagerKeynodes::rrel_sets}}};

  componentsClasses = {
      {keynodes::ScComponentManagerKeynodes::concept_reusable_ui_component,
       keynodes::ScComponentManagerKeynodes::sc_model_of_interface},
      {keynodes::ScComponentManagerKeynodes::concept_reusable_ps_component,
       keynodes::ScComponentManagerKeynodes::sc_model_of_problem_solver},
      {keynodes::ScComponentManagerKeynodes::concept_reusable_kb_component,
       keynodes::ScComponentManagerKeynodes::sc_model_of_knowledge_base},
      {keynodes::ScComponentManagerKeynodes::concept_reusable_embedded_ostis_system,
       keynodes::ScComponentManagerKeynodes::concept_subsystems_set}};
}

// actionAddr -> relation(flag)': set (* -> component1;; -> component2;; *)
// actionAddr -> relation(flag)': set (* -> subflag(subrelation)' : subset1;; -> subflag' : subset2;; *)
ScAddr GetSetAddrOfComponents(ScMemoryContext & context, ScAddr const & actionAddr, ScAddr const & parameterRelNodeAddr)
{
  ScAddr relationAddr;
  ScAddr endSetAddr;
  ScAddr setAddr;
  for (auto & relationAndSubRelations : CommonUtils::mainFlagWithSubFlags)
  {
    if (std::find(relationAndSubRelations.second.begin(), relationAndSubRelations.second.end(), parameterRelNodeAddr)
        != relationAndSubRelations.second.end())
    {
      // If main parameter consists of several parameters
      relationAddr = context.HelperFindBySystemIdtf(relationAndSubRelations.first);
      break;
    }
  }
  if (context.IsElement(relationAddr))
  {
    // Check if the main parameter is already created to avoid duplicating
    setAddr = utils::IteratorUtils::getAnyByOutRelation(&context, actionAddr, relationAddr);
    if (!context.IsElement(setAddr))
    {
      setAddr = context.CreateNode(ScType::NodeConst);
    }
    ScAddr subSetAddr = context.CreateNode(ScType::NodeConst);
    utils::GenerationUtils::generateRelationBetween(&context, actionAddr, setAddr, relationAddr);
    utils::GenerationUtils::generateRelationBetween(&context, setAddr, subSetAddr, parameterRelNodeAddr);
    endSetAddr = subSetAddr;
  }
  else
  {
    setAddr = context.CreateNode(ScType::NodeConst);
    utils::GenerationUtils::generateRelationBetween(&context, actionAddr, setAddr, parameterRelNodeAddr);
    endSetAddr = setAddr;
  }
  // Return Addr with which we connect components
  return endSetAddr;
}

bool CommonUtils::TranslateFromStringToScMemory(
    ScMemoryContext & context,
    ScAddr const & actionAddr,
    std::map<std::string, std::vector<std::string>> const & commandParameters)
{
  ScAddr parameterValueAddr;
  ScAddr parameterRelNodeAddr;
  for (auto const & parameter : commandParameters)
  {
    try
    {
      parameterRelNodeAddr = managerParametersWithAgentRelations.at(parameter.first);
    }
    catch (std::out_of_range const & exception)
    {
      SC_LOG_INFO("Transform to sc-structure: Unknown parameter " << parameter.first);
      continue;
    }

    ScAddr const & setAddr = GetSetAddrOfComponents(context, actionAddr, parameterRelNodeAddr);

    for (std::string const & parameterValue : parameter.second)
    {
      if (parameter.first == CommandsSearchFlags::EXPLANATION ||
          parameter.first == CommandsSearchFlags::NOTE ||
          parameter.first == CommandsSearchFlags::PURPOSE ||
          parameter.first == CommandsSearchFlags::MAIN_ID)
      {
        parameterValueAddr = context.CreateNode(ScType::LinkConst);
        context.SetLinkContent(parameterValueAddr, parameterValue);
      }
      else
      {
        parameterValueAddr = context.HelperFindBySystemIdtf(parameterValue);
        if (!context.IsElement(parameterValueAddr))
        {
          SC_LOG_WARNING("Transform to sc-structure: Unknown value: " << parameterValue);
          parameterValueAddr = context.CreateNode(ScType::NodeConst);
          context.HelperSetSystemIdtf(parameterValue, parameterValueAddr);
        }
      }
      context.CreateEdge(ScType::EdgeAccessConstPosPerm, setAddr, parameterValueAddr);
    }
  }
  return true;
}

ScAddrUnorderedSet CommonUtils::GetComponentsToInstall(ScMemoryContext & context, ScAddr const & parameterNode)
{
  ScAddrUnorderedSet components;
  if (!context.IsElement(parameterNode))
  {
    return components;
  }

  ScAddr const & parameterSetNode = utils::IteratorUtils::getAnyByOutRelation(
      &context, parameterNode, keynodes::ScComponentManagerKeynodes::rrel_sets);
  ScAddr const & parameterComponentsNode = utils::IteratorUtils::getAnyByOutRelation(
      &context, parameterNode, keynodes::ScComponentManagerKeynodes::rrel_components);

  if (context.IsElement(parameterSetNode))
  {
    ScIterator3Ptr const & setsIterator =
        context.Iterator3(parameterSetNode, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    ScAddr componentClassAddr;
    while (setsIterator->Next())
    {
      componentClassAddr = setsIterator->Get(2);
      ScIterator3Ptr const & componentsIterator =
          context.Iterator3(componentClassAddr, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
      while (componentsIterator->Next())
      {
        components.insert(componentsIterator->Get(2));
      }
    }
  }

  if (context.IsElement(parameterComponentsNode))
  {
    ScIterator3Ptr const & componentsIterator =
        context.Iterator3(parameterComponentsNode, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (componentsIterator->Next())
    {
      components.insert(componentsIterator->Get(2));
    }
  }
  return components;
}

std::map<std::string, ScAddr> CommonUtils::GetSetElements(ScMemoryContext & context, ScAddr const & setAddr)
{
  std::map<std::string, ScAddr> elementsIdtfAndAddr;
  if (!context.IsElement(setAddr))
    return elementsIdtfAndAddr;
  ScIterator3Ptr const & elementsIterator =
      context.Iterator3(setAddr, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
  while (elementsIterator->Next())
  {
    try
    {
      elementsIdtfAndAddr.insert({context.HelperGetSystemIdtf(elementsIterator->Get(2)), elementsIterator->Get(2)});
    }
    catch (std::exception const & exception)
    {
      SC_LOG_WARNING("CommonUtils::GetSetElements : met element without system identifier");
      continue;
    }
  }
  return elementsIdtfAndAddr;
}

std::map<std::string, ScAddr> CommonUtils::GetElementsLinksOfSet(ScMemoryContext & context, ScAddr const & setAddr)
{
  std::map<std::string, ScAddr> elementsIdtfAndAddr;
  if (!context.IsElement(setAddr))
    return elementsIdtfAndAddr;
  std::string elementIdtf;
  ScIterator3Ptr const & elementsIterator =
      context.Iterator3(setAddr, ScType::EdgeAccessConstPosPerm, ScType::LinkConst);
  while (elementsIterator->Next())
  {
    context.GetLinkContent(elementsIterator->Get(2), elementIdtf);
    elementsIdtfAndAddr.insert({elementIdtf, elementsIterator->Get(2)});
  }
  return elementsIdtfAndAddr;
}

ScAddr CommonUtils::GetSubsystemDecompositionAddr(ScMemoryContext & context, ScAddr const & component)
{
  ScAddr componentDecomposition;
  ScAddr componentClass;
  if (!context.IsElement(component))
  {
    return componentDecomposition;
  }

  for (std::vector<ScAddr> const & commonComponentClass : common_utils::CommonUtils::componentsClasses)
  {
    if (context.HelperCheckEdge(commonComponentClass[0], component, ScType::EdgeAccessConstPosPerm))
    {
      componentClass = commonComponentClass[1];
      break;
    }
  }
  ScAddr myselfDecomposition = utils::IteratorUtils::getAnyByOutRelation(
      &context,
      keynodes::ScComponentManagerKeynodes::myself,
      keynodes::ScComponentManagerKeynodes::nrel_ostis_system_decomposition);

  if (!context.IsElement(myselfDecomposition))
  {
    return componentDecomposition;
  }
  ScIterator3Ptr partsDecomposition =
      context.Iterator3(myselfDecomposition, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);

  while (partsDecomposition->Next())
  {
    if (!context.HelperCheckEdge(componentClass, partsDecomposition->Get(2), ScType::EdgeAccessConstPosPerm))
      continue;

    componentDecomposition = utils::IteratorUtils::getAnyByOutRelation(
        &context, partsDecomposition->Get(2), keynodes::ScComponentManagerKeynodes::nrel_decomposition);
    break;
  }

  return componentDecomposition;
}

bool CommonUtils::CheckIfInstalled(ScMemoryContext & context, ScAddr const & component)
{
  ScAddr decompositionAddr = GetSubsystemDecompositionAddr(context, component);
  if (!context.IsElement(decompositionAddr) || !context.IsElement(component))
    return false;
  return context.HelperCheckEdge(decompositionAddr, component, ScType::EdgeAccessConstPosPerm);
}

ScAddr CommonUtils::GetComponentBySpecification(ScMemoryContext & context, ScAddr const & specification)
{
  if (!context.IsElement(specification))
    return specification;
  return utils::IteratorUtils::getAnyByOutRelation(
      &context, specification, scAgentsCommon::CoreKeynodes::rrel_key_sc_element);
}

bool CommonUtils::CheckIfFullMyselfDecompositionExists(ScMemoryContext & context)
{
  ScAddr myselfDecompositionAddr = GetMyselfDecompositionAddr(context);
  if (!context.IsElement(myselfDecompositionAddr))
    return false;

  ScAddr componentClassAddr;
  ScAddr myselfDecompositionPartAddr;
  ScAddr partDecompositionAddr;

  for (ScAddrVector const & subsystemAndComponentClass : componentsClasses)
  {
    componentClassAddr = subsystemAndComponentClass[1];

    ScIterator3Ptr it = context.Iterator3(myselfDecompositionAddr, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (it->Next())
    {
      myselfDecompositionPartAddr = it->Get(2);
      if (context.HelperCheckEdge(componentClassAddr, myselfDecompositionPartAddr, ScType::EdgeAccessConstPosPerm))
      {
        partDecompositionAddr = utils::IteratorUtils::getAnyByOutRelation(
            &context, myselfDecompositionPartAddr, keynodes::ScComponentManagerKeynodes::nrel_decomposition);
        if (!context.IsElement(partDecompositionAddr))
          return false;
      }
    }
  }
  return true;
}

}  // namespace common_utils
