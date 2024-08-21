/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager_command_handler.hpp"

#include <sc-agents-common/keynodes/coreKeynodes.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>
#include <sc-agents-common/utils/GenerationUtils.hpp>

#include <sc-memory/sc_scs_helper.hpp>

#include "common-module/module/utils/common_utils.hpp"
#include "common-module/module/keynodes/ScComponentManagerKeynodes.hpp"
#include "constants/command_constants.hpp"


class DummyFileInterface : public SCsFileInterface
{
public:
  ~DummyFileInterface() override {}

  ScStreamPtr GetFileContent(std::string const &) override
  {
    return ScStreamPtr();
  }
};

ScComponentManagerCommandHandler::ScComponentManagerCommandHandler()
{
  m_context = new ScMemoryContext();

  m_actions = {
      {CommandConstants::COMPONENTS_COMMAND_SEARCH, keynodes::ScComponentManagerKeynodes::action_components_search},
      {CommandConstants::COMPONENTS_COMMAND_INSTALL, keynodes::ScComponentManagerKeynodes::action_components_install},
      {CommandConstants::COMPONENTS_COMMAND_INIT, keynodes::ScComponentManagerKeynodes::action_components_init}};

  m_installParametersRelations = {
      {CommandsSearchFlags::IDTF, keynodes::ScComponentManagerKeynodes::rrel_components},
      {CommandsSearchFlags::SET, keynodes::ScComponentManagerKeynodes::rrel_sets}};

  m_searchQuasybinaryRelations = {
      {CommandsSearchFlags::AUTHOR, keynodes::ScComponentManagerKeynodes::nrel_authors}};

  m_searchLinksRelations = {
      {CommandsSearchFlags::EXPLANATION, keynodes::ScComponentManagerKeynodes::nrel_explanation},
      {CommandsSearchFlags::IDTF, scAgentsCommon::CoreKeynodes::nrel_idtf},
      {CommandsSearchFlags::MAIN_ID, scAgentsCommon::CoreKeynodes::nrel_main_idtf},
      {CommandsSearchFlags::NOTE, keynodes::ScComponentManagerKeynodes::nrel_note},
      {CommandsSearchFlags::PURPOSE, keynodes::ScComponentManagerKeynodes::nrel_purpose}
    };
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
    FormInstallActionNodeParameter(action, commandParameters, CommandsSearchFlags::SET);
    FormInstallActionNodeParameter(action, commandParameters, CommandsSearchFlags::IDTF);
  }
  else if (actionsIterator->first == CommandConstants::COMPONENTS_COMMAND_SEARCH)
  {
    FormSearchActionNodeParameter(action, commandParameters);
  }

  utils::AgentUtils::applyAction(m_context, action, 30000);

  bool const executionResult = m_context->HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::action_finished_successfully, action, ScType::EdgeAccessConstPosPerm);

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
    relation = m_installParametersRelations.at(parameter);
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
  ScAddr const searchStructure = m_context->CreateNode(ScType::NodeConstStruct);
  std::string scsTemplate = "concept_reusable_component _-> .._component;;\n";
  ScAddr parameterValue;
  size_t index = 1;
  std::string PARAMETERS_SET_ALIAS = ".._parameters_set";
  for (auto const & commandParams : commandParameters)
  {
    PARAMETERS_SET_ALIAS += std::to_string(index);
    index++;

    // .._component _=> nrel_quasibinary_relation:: .._parameters_set(index) (* _-> param1; _-> param2;; *);;
    if (m_searchQuasybinaryRelations.find(commandParams.first) != m_searchQuasybinaryRelations.cend())
    {
      scsTemplate += ".._component _=> " + m_context->HelperGetSystemIdtf(m_searchQuasybinaryRelations.at(commandParams.first)) + ":: " + PARAMETERS_SET_ALIAS + "(* ";
      for (std::string const & commandParameterValue : commandParams.second)
      {
        parameterValue = m_context->HelperFindBySystemIdtf(commandParameterValue);
        if (!m_context->IsElement(parameterValue))
        {
          SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "ScComponentManagerCommandHandler: parameter with value " + commandParameterValue + " is not found.");
        }
        scsTemplate += " _-> " + commandParameterValue + ";";
      }
      scsTemplate += "; *);;\n";
    }

    // _component _=> nrel_binary_relation:: _[link with content]
    else if (m_searchLinksRelations.find(commandParams.first) != m_searchLinksRelations.cend())
    {
      for (std::string const & commandParameterValue : commandParams.second)
      {
        scsTemplate += ".._component _=> " + m_context->HelperGetSystemIdtf(m_searchLinksRelations.at(commandParams.first)) + ":: _[" + commandParameterValue + "];;\n";
      }
    }
    // concept_some_component_class _-> _component
    else if (commandParams.first == CommandsSearchFlags::CLASS)
    {
      for (std::string const & commandParameterValue : commandParams.second)
      {
        parameterValue = m_context->HelperFindBySystemIdtf(commandParameterValue);
        if (!m_context->IsElement(parameterValue))
        {
          SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "ScComponentManagerCommandHandler: class " + commandParameterValue + " is not found.");
        }
        scsTemplate += commandParameterValue + "_-> .._component;;";
      }
    }
  }

  SC_LOG_WARNING(scsTemplate);
  
  SCsHelper helper(*m_context, std::make_shared<DummyFileInterface>());
  helper.GenerateBySCsTextLazy(scsTemplate, searchStructure);

  // Add search structure as an argument to search action node
  utils::GenerationUtils::generateRelationBetween(m_context, action, searchStructure, scAgentsCommon::CoreKeynodes::rrel_1);
}

ScComponentManagerCommandHandler::~ScComponentManagerCommandHandler()
{
  m_context->Destroy();
  delete m_context;

  m_actions.clear();
}
