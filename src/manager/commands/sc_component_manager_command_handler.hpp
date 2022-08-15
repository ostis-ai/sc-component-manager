/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <utility>

#include "sc_component_manager_handler.hpp"
#include "sc_component_manager_command.hpp"
#include "src/manager/commands/command_init/sc_component_manager_command_init.hpp"

class ScComponentManagerCommandHandler : public ScComponentManagerHandler
{
public:
  explicit ScComponentManagerCommandHandler(std::string reposPath, std::string specificationsPath)
    : m_reposPath(std::move(reposPath)), m_specificationsPath(std::move(specificationsPath))
  {
    m_context = new ScMemoryContext("sc-component-manager-command-handler");
  }

  ExecutionResult Handle(std::string const & commandType, CommandParameters const & commandParameters) override
  {
    ExecutionResult executionResult;

    auto const & it = m_actions.find(commandType);
    if (it != m_actions.end())
    {
      ScComponentManagerCommand * commander = it->second;
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

  ~ScComponentManagerCommandHandler() override
  {
    m_context->Destroy();
    delete m_context;

    for (auto const & it : m_actions)
      delete it.second;

    m_actions.clear();
  }

protected:
  ScMemoryContext * m_context{};

  std::string m_reposPath;
  std::string m_specificationsPath;

  std::map<std::string, ScComponentManagerCommand *> m_actions = {
      { "init", new ScComponentManagerCommandInit(m_reposPath, m_specificationsPath) }
  };
};
