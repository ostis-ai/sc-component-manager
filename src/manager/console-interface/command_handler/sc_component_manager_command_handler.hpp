/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "commands/sc_component_manager_command.hpp"

class ScComponentManagerCommandHandler
{
public:
  ScComponentManagerCommandHandler();

  /*!
   * @brief Find action for the corresponding command and call agent
   * @param commandType type of the command (init, install or search)
   * @param commandParameters parameters of command (identifier, author, class...)
   * @return true if action finished successfully, otherwise false
   */
  bool Handle(std::string const & commandType, CommandParameters const & commandParameters) noexcept(false);

  ~ScComponentManagerCommandHandler();

protected:
  ScMemoryContext * m_context{};
  std::map<std::string, ScAddr> m_actions;
  std::map<std::string, ScAddr> m_InstallParametersRelations;
  std::map<std::string, ScAddr> m_SearchParametersRelations;

  void FormInstallActionNodeParameter(
      ScAddr const & action,
      CommandParameters const & commandParameters,
      std::string const & parameter);

  void FormSearchActionNodeParameter(ScAddr const & action, CommandParameters const & commandParameters);
};
