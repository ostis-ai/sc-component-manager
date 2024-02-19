/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>
#include <iostream>
#include <algorithm>
#include <regex>

#include "../commands/sc_component_manager_command.hpp"
#include "../commands/constants/command_constants.hpp"

class ScComponentManagerParser
{
public:
  static std::pair<std::string, CommandParameters> Parse(std::string const & command)
  {
    size_t const COMMAND_KEYWORDS_SIZE = 2;

    std::string cutCommand;
    size_t firstPapameter = command.find('-');
    if (firstPapameter != std::string::npos)
    {
      cutCommand = command.substr(0, firstPapameter - 1);
    }
    else {
      cutCommand = command;

      // From "cinst sc_web" to "{"cinst, "sc_web"}
      std::stringstream commandsStream(command);
      std::vector<std::string> tokens;
      while (!commandsStream.eof()) {
        std::string substring;
        commandsStream >> substring;
        tokens.push_back(substring);
      }

      // Try to find abbreviation of command, "cinst" or "cinst sc_web"
      std::stringstream subCommand;
      for (std::string & token: tokens) {
        subCommand << token;
        for (auto & listOfCommands: CommandConstants::COMMAND_LIST)
          for (size_t j = 0; j < listOfCommands.size(); j++)
            if (std::find(listOfCommands.begin(), listOfCommands.end(), subCommand.str()) != listOfCommands.end())
              cutCommand = subCommand.str(); // "cinst" is element of abbreviation list
      }
    }

    size_t endOfCommandPos = cutCommand.find_last_not_of(' ');
    if (endOfCommandPos != std::string::npos)
    {
      cutCommand.erase(endOfCommandPos + 1);
    }

    std::string fullCommand = command;
    for (size_t indexCommand = 0; indexCommand < CommandConstants::COMMAND_LIST.size(); indexCommand++)
    {
      for (size_t indexReducedCommand = 1; indexReducedCommand < CommandConstants::COMMAND_LIST[indexCommand].size();
           indexReducedCommand++)
      {
        if (cutCommand == CommandConstants::COMMAND_LIST[indexCommand][indexReducedCommand])
        {
          fullCommand.replace(0, cutCommand.size(), CommandConstants::COMMAND_LIST[indexCommand][0]);
          indexCommand = CommandConstants::COMMAND_LIST.size();
          break;
        }
      }
    }

    std::pair<std::string, CommandParameters> parsedCommand;
    std::vector<std::string> commandTokens;
    commandTokens = ParseCommand(fullCommand);

    if (commandTokens.size() < COMMAND_KEYWORDS_SIZE)
      SC_THROW_EXCEPTION(
          utils::ExceptionParseError, "Incorrect command. Command type not found in \"" + command + "\"");

    if (commandTokens.at(0) != CommandConstants::COMPONENTS_COMMAND_PREFIX)
      SC_THROW_EXCEPTION(
          utils::ExceptionParseError,
          "\"" + command + "\" is not a command. Maybe you mean " + CommandConstants::COMPONENTS_COMMAND_PREFIX + "?");

    parsedCommand.first = commandTokens.at(1);
    CommandParameters commandParameters = GetCommandParameters(commandTokens);
    parsedCommand.second = commandParameters;

    return parsedCommand;
  }

protected:
  static CommandParameters GetCommandParameters(std::vector<std::string> const & commandTokens)
  {
    size_t const COMMAND_KEYWORDS_SIZE = 2;
    char const PARAMETER_VALUES_DELIMITER = '-';

    CommandParameters commandParameters;
    std::string parameterName;
    std::vector<std::string> parameterValue;

    for (size_t tokenNumber = COMMAND_KEYWORDS_SIZE; tokenNumber < commandTokens.size(); tokenNumber++)
    {
      std::string currentCommandToken = commandTokens.at(tokenNumber);
      if (currentCommandToken.at(0) == PARAMETER_VALUES_DELIMITER)
      {
        if (!parameterValue.empty())
        {
          commandParameters.insert({parameterName, parameterValue});
          parameterValue.clear();
        }
        parameterName = GetParameterNameAfterDelimiter(currentCommandToken, PARAMETER_VALUES_DELIMITER);
      }
      else
      {
        if (currentCommandToken.at(0) == '\"')
        {
          currentCommandToken = currentCommandToken.substr(1, currentCommandToken.size() - 2);
        }
        parameterValue.push_back(currentCommandToken);
      }
    }

    if (!parameterName.empty())
    {
      commandParameters.insert({parameterName, parameterValue});
    }
    else
    {
      std::stringstream fullCommand;
      fullCommand << commandTokens[0] << " " << commandTokens[1];
      if (fullCommand.str() == CommandConstants::COMMAND_COMPONENTS_INSTALL[0] && !parameterValue.empty())
      {
        commandParameters.insert({CommandsConstantsFlags::IDTF, parameterValue});
      }
    }

    InsertParametersWithoutValues(commandParameters, commandTokens, PARAMETER_VALUES_DELIMITER);

    return commandParameters;
  }

  static std::string GetParameterNameAfterDelimiter(
      std::string const & currentCommandToken,
      char const parameterDelimiter)
  {
    if (currentCommandToken.at(1) == parameterDelimiter)
      return currentCommandToken.substr(2);
    else
      return currentCommandToken.substr(1);
  }

  static void InsertParametersWithoutValues(
      CommandParameters & commandParameters,
      std::vector<std::string> const & commandTokens,
      char const parameterDelimiter)
  {
    std::string parameterName;
    std::vector<std::string> commandParametersName;

    for (auto & it : commandParameters)
    {
      commandParametersName.push_back(it.first);
    }

    for (std::string const & commandToken : commandTokens)
    {
      parameterName = GetParameterName(commandToken, parameterDelimiter);
      if (!parameterName.empty())
      {
        if (std::find(std::begin(commandParametersName), std::end(commandParametersName), parameterName) ==
            std::end(commandParametersName))
          commandParameters.insert({parameterName, {}});
      }
    }
  }

  static std::string GetParameterName(std::string const & commandToken, char const parameterDelimiter)
  {
    std::string parameterName;
    if (commandToken.at(0) == parameterDelimiter)
    {
      parameterName = GetParameterNameAfterDelimiter(commandToken, parameterDelimiter);
    }

    return parameterName;
  }

  static std::vector<std::string> ParseCommand(std::string const & command)
  {
    std::vector<std::string> result;

    std::regex regex(R"(("[^"]+"|[^\s"]+))");
    auto commandBegin = std::sregex_iterator(command.begin(), command.end(), regex);
    auto commandEnd = std::sregex_iterator();
    for (std::sregex_iterator i = commandBegin; i != commandEnd; ++i)
    {
      std::smatch const & match = *i;
      std::string matchStr = match.str();
      result.push_back(matchStr);
    }

    return result;
  }
};
