/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager_command_handler.hpp"

#include <sc-agents-common/keynodes/coreKeynodes.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>
#include <sc-agents-common/utils/GenerationUtils.hpp>

#include "common-module/module/utils/common_utils.hpp"
#include "common-module/module/keynodes/ScComponentManagerKeynodes.hpp"
#include "constants/command_constants.hpp"

ScComponentManagerCommandHandler::ScComponentManagerCommandHandler()
{
  m_context = new ScMemoryContext();

  m_actions = {
      {CommandConstants::COMPONENTS_COMMAND_SEARCH, keynodes::ScComponentManagerKeynodes::action_components_search},
      {CommandConstants::COMPONENTS_COMMAND_INSTALL, keynodes::ScComponentManagerKeynodes::action_components_install},
      {CommandConstants::COMPONENTS_COMMAND_INIT, keynodes::ScComponentManagerKeynodes::action_components_init}};

  m_InstallParametersRelations = {
      {CommandsConstantsFlags::IDTF, keynodes::ScComponentManagerKeynodes::rrel_components},
      {CommandsConstantsFlags::SET, keynodes::ScComponentManagerKeynodes::rrel_sets},
      {CommandsConstantsFlags::AUTHOR, keynodes::ScComponentManagerKeynodes::rrel_author},
      {CommandsConstantsFlags::CLASS, keynodes::ScComponentManagerKeynodes::rrel_class},
      {CommandsConstantsFlags::EXPLANATION, keynodes::ScComponentManagerKeynodes::rrel_explanation}};

  m_SearchParametersRelations = {
      {CommandsConstantsFlags::AUTHOR, keynodes::ScComponentManagerKeynodes::rrel_author},
      {CommandsConstantsFlags::CLASS, keynodes::ScComponentManagerKeynodes::rrel_class},
      {CommandsConstantsFlags::EXPLANATION, keynodes::ScComponentManagerKeynodes::rrel_explanation}};
}

bool ScComponentManagerCommandHandler::Handle(
    std::string const & commandType,
    CommandParameters const & commandParameters)
{
  auto const & actionsIterator = m_actions.find(commandType);
  if (actionsIterator == m_actions.end())
  {
    SC_THROW_EXCEPTION(utils::ExceptionParseError, "ScComponentManagerCommandHandler: Unsupported command type \"" + commandType + "\"");
  }

  SC_LOG_DEBUG("ScComponentManagerCommandHandler: execute " + actionsIterator->first + " command");
  ScAddr const actionClass = actionsIterator->second;
  ScAddr const action = utils::AgentUtils::formActionNode(m_context, actionClass, {});

  if (actionsIterator->first == CommandConstants::COMPONENTS_COMMAND_INSTALL)
  {
    FormInstallActionNodeParameter(action, commandParameters, CommandsConstantsFlags::SET);
    FormInstallActionNodeParameter(action, commandParameters, CommandsConstantsFlags::IDTF);
  }
  else if (actionsIterator->first == CommandConstants::COMPONENTS_COMMAND_SEARCH)
  {
    FormSearchActionNodeParameter(action, commandParameters);
  }

  common_utils::CommonUtils::TranslateFromStringToScMemory(*m_context, action, commandParameters);

  utils::AgentUtils::applyAction(m_context, action, 30000);

  bool const executionResult = m_context->HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_successfully, action, ScType::EdgeAccessConstPosPerm);

  return executionResult;
}

void ScComponentManagerCommandHandler::FormInstallActionNodeParameter(
    ScAddr const & action,
    CommandParameters const & commandParameters,
    std::string const & parameter)
{
  std::vector<std::string> const & identifiers = commandParameters.at(parameter);
  if (identifiers.empty())
    return;

  ScAddr relation;
  try
  {
    relation = m_InstallParametersRelations.at(parameter);
  }
  catch (std::out_of_range const & exception)
  {
    SC_LOG_WARNING("ScComponentManagerCommandHandler: Unknown parameter " << parameter);
  }

  ScAddr const componentsSet = m_context->CreateNode(ScType::NodeConst);
  utils::GenerationUtils::generateRelationBetween(m_context, action, componentsSet, relation);

  ScAddr component;
  for (std::string const & identifier : identifiers)
  {
    component = m_context->HelperFindBySystemIdtf(identifier);
    if (m_context->IsElement(component))
    {
      m_context->CreateEdge(ScType::EdgeAccessConstPosPerm, componentsSet, component);
    }
    else
    {
      SC_LOG_WARNING(
          "ScComponentManagerCommandHandler: " << identifier << " component (or set of components) not found.");
    }
  }
}

void ScComponentManagerCommandHandler::FormSearchActionNodeParameter(ScAddr const & action, CommandParameters const & commandParameters)
{
  ScAddr paramsSet;
  for (auto const & param : commandParameters)
  {
    if (m_SearchParametersRelations.find(param.first) != m_SearchParametersRelations.cend())
    {
      paramsSet = m_context->CreateNode(ScType::NodeConst);
    }
  }
}

ScComponentManagerCommandHandler::~ScComponentManagerCommandHandler()
{
  m_context->Destroy();
  delete m_context;

  m_actions.clear();
}
