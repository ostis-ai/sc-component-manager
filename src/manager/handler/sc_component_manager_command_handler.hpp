/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc_component_manager_handler.hpp"
#include "sc_component_manager_command.hpp"
#include "sc_component_manager_command_init.hpp"

class ScComponentManagerCommandHandler : public ScComponentManagerHandler
{
public:
  ExecutionResult Handle(std::string const & commandType, CommandParameters const & commandParameters) override
  {
    ExecutionResult executionResult;

    auto const & it = m_actions.find(commandType);
    if (it != m_actions.end())
    {
      ScComponentManagerCommand * commander = it->second;
      commander->SetReposPath(m_reposPath);
      std::cout << "ScComponentManagerCommandHandler: execute " + it->first + " command\n";
      SC_LOG_DEBUG("ScComponentManagerCommandHandler: execute " + it->first + " command");
      executionResult = commander->Execute(m_context, commandParameters);
    }
    else
    {
      executionResult = {"Unsupported command type: \"" + commandType + "\""};
    }

    return executionResult;
  }

  ScComponentManagerCommandHandler()
  {
    m_context = new ScMemoryContext("sc-component-manager-command-handler");
  }

  ~ScComponentManagerCommandHandler() override
  {
    m_context->Destroy();
    delete m_context;

    for (auto const & it : m_actions)
      delete it.second;
  }

protected:
  ScMemoryContext * m_context;

  std::map<std::string, ScComponentManagerCommand *> m_actions = {{"init", new ScComponentManagerCommandInit()}};
};
