/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>
#include <iostream>

#include "../handler/sc_component_manager_command.hpp"

#include <algorithm>

class ScComponentManagerParser
{
public:
  static std::pair<std::string, CommandParameters> Parse(std::string const & command)
  {
    std::string const COMPONENTS_COMMAND_PREFIX = "components";
    std::pair<std::string, CommandParameters> parsedCommand;
    std::vector<std::string> commandTokens;
    utils::StringUtils::SplitString(command, ' ', commandTokens);

    if (commandTokens.size() < 2)
      SC_THROW_EXCEPTION(
          utils::ExceptionParseError, "Incorrect command. Command type not found in \"" + command + "\"");

    if (commandTokens.at(0) != COMPONENTS_COMMAND_PREFIX)
      SC_THROW_EXCEPTION(
          utils::ExceptionParseError,
          "\"" + command + "\" is not a command. Maybe you mean " + COMPONENTS_COMMAND_PREFIX + "?");

    parsedCommand.first = commandTokens.at(1);

    SC_LOG_DEBUG("ScComponentManagerParser: parsed command " + parsedCommand.first);

    CommandParameters commandParameters = GetCommandParameters(commandTokens);

    parsedCommand.second = commandParameters;

    return parsedCommand;
  }

protected:
  static CommandParameters GetCommandParameters(std::vector<std::string> & commandTokens)
  {
    CommandParameters commandParameters;
    std::string parameterName;
    std::vector<std::string> parameterValue;

    for (size_t tokenNumber = 2; tokenNumber < commandTokens.size(); tokenNumber++)
    {
      if (commandTokens.at(tokenNumber).at(0) == '-')
      {
        if (!parameterValue.empty())
        {
          commandParameters.insert({parameterName, parameterValue});
          //
          std::string values;
          for (std::string const & value : parameterValue)
          {
            values += " " + value;
          }
          SC_LOG_DEBUG("ScComponentManagerParser: parsed parameter " + parameterName + " with value" + values);
          //
          parameterValue.clear();
        }
        if (commandTokens.at(tokenNumber).at(1) == '-')
          parameterName = commandTokens.at(tokenNumber).substr(2);
        else
          parameterName = commandTokens.at(tokenNumber).substr(1);
      }
      else
        parameterValue.push_back(commandTokens.at(tokenNumber));
    }
    if (!parameterName.empty())
      commandParameters.insert({parameterName, parameterValue});

    std::vector<std::string> commandParametersName;
    for (auto & it : commandParameters)
    {
      commandParametersName.push_back(it.first);
    }

    for (std::string const & commandToken : commandTokens)
    {
      parameterName = GetParameterName(commandToken);
      if (!parameterName.empty())
      {
        if (std::find(std::begin(commandParametersName), std::end(commandParametersName), parameterName) ==
            std::end(commandParametersName))
          commandParameters.insert({parameterName, {}});
      }
    }

    return commandParameters;
  }

  static std::string GetParameterName(std::string const & commandToken)
  {
    std::string parameterName;
    if (commandToken.at(0) == '-')
    {
      if (commandToken.at(1) == '-')
        parameterName = commandToken.substr(2);
      else
        parameterName = commandToken.substr(1);
    }

    return parameterName;
  }
};
