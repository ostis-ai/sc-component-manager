/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <utility>

#include <sc-memory/sc_agent_context.hpp>

#include <sc-agents-common/utils/CommonUtils.hpp>

#include <common/common_utils.hpp>
#include <common/sc_component_manager_keynodes.hpp>
#include <common/sc_component_manager_command.hpp>

class ScComponentManagerCommandHandler
{
public:
  explicit ScComponentManagerCommandHandler()
  {
    m_context = new ScAgentContext();
    m_actions = {"init", "search", "install"};
  }

  bool Handle(std::string const & commandType, CommandParameters const & commandParameters)
  {
    bool executionResult;
    m_commandParameters = commandParameters;
    auto const & it = m_actions.find(commandType);
    if (it != m_actions.end())
    {
      ScAddr actionAddrClass;
      try
      {
        actionAddrClass = common_utils::CommonUtils::managerParametersWithAgentRelations.at(*it);
        SC_LOG_DEBUG("ScComponentManagerCommandHandler: execute " << *it << " command");
      }
      catch (std::out_of_range const & ex)
      {
        SC_LOG_ERROR(ex.what());
      }
      ScAction actionAddr = m_context->GenerateAction(actionAddrClass);
      common_utils::CommonUtils::TranslateFromStringToScMemory(*m_context, actionAddr, commandParameters);

      actionAddr.InitiateAndWait(30000);

      executionResult = actionAddr.IsFinishedSuccessfully();
    }
    else
    {
      SC_THROW_EXCEPTION(utils::ExceptionParseError, "Unsupported command type \"" + commandType + "\"");
    }

    return executionResult;
  }

  ~ScComponentManagerCommandHandler()
  {
    m_context->Destroy();
    delete m_context;

    m_actions.clear();
  }

protected:
  ScAgentContext * m_context{};
  CommandParameters m_commandParameters;
  std::set<std::string> m_actions;
};
