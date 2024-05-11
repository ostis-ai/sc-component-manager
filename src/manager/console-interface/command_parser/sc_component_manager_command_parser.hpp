/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <regex>

#include "common-module/commands/sc_component_manager_command.hpp"
#include "common-module/constants/command_constants.hpp"

class ScComponentManagerParser
{
public:
  static std::pair<std::string, CommandParameters> Parse(std::string const & command)
  {
    size_t const COMMAND_KEYWORDS_SIZE = 2;
    std::string const fullCommand = GetFullCommand(command);
    //    std::string cutCommand = fullCommand;
    //
    //    size_t firstPapameter = fullCommand.find('-');
    //    if (firstPapameter != std::string::npos)
    //    {
    //      cutCommand = fullCommand.substr(0, firstPapameter - 1);
    //    }
    //
    //    size_t endOfCommandPos = cutCommand.find_last_not_of(' ');
    //    if (endOfCommandPos != std::string::npos)
    //    {
    //      cutCommand.erase(endOfCommandPos + 1);
    //    }
    //    SC_LOG_WARNING(cutCommand);
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
    //    for (auto & p : parsedCommand.second)
    //    {
    //      SC_LOG_WARNING(p.first);
    //      for (auto & x : p.second)
    //      {
    //        SC_LOG_WARNING(":\t" << x);
    //      }
    //    }

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
    std::string currentCommandToken;

    for (size_t tokenNumber = COMMAND_KEYWORDS_SIZE; tokenNumber < commandTokens.size(); tokenNumber++)
    {
      currentCommandToken = commandTokens.at(tokenNumber);
      if (tokenNumber == COMMAND_KEYWORDS_SIZE && commandTokens[0] == CommandConstants::COMPONENTS_COMMAND_PREFIX
          && commandTokens[1] == CommandConstants::COMPONENTS_COMMAND_INSTALL
          && currentCommandToken.at(0) != PARAMETER_VALUES_DELIMITER)
      {
        parameterName = CommandsConstantsFlags::IDTF;
      }
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
    //    else
    //    {
    //      if (commandTokens[0] == CommandConstants::COMPONENTS_COMMAND_PREFIX
    //          && commandTokens[1] == CommandConstants::COMPONENTS_COMMAND_INSTALL && !parameterValue.empty())
    //      {
    //        commandParameters.insert({CommandsConstantsFlags::IDTF, parameterValue});
    //      }
    //    }

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
        if (std::find(std::begin(commandParametersName), std::end(commandParametersName), parameterName)
            == std::end(commandParametersName))
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

  static std::vector<std::string> DivideStrIntoLexems(std::string const & command)
  {
    std::stringstream commandsStream(command);
    std::vector<std::string> tokens;
    while (!commandsStream.eof())
    {
      std::string substring;
      commandsStream >> substring;
      tokens.push_back(substring);
    }
    return tokens;
  }

  static std::string DeleteMultipleSpaces(std::string const & command)
  {
    std::string shortCommand;
    for (size_t index = 0; index < command.size() - 1; index++)
    {
      if (command[index] == ' ' && command[index + 1] != ' ')
      {
        shortCommand.push_back(command[index]);
        continue;
      }

      shortCommand.push_back(command[index]);
    }
    if (command[command.size() - 1] != ' ')
    {
      shortCommand.push_back(command[command.size() - 1]);
    }

    return shortCommand;
  }

  static std::string GetFullCommand(std::string const & command)
  {
    std::string fullCommand = DeleteMultipleSpaces(command);
    std::stringstream subCommand;
    std::vector<std::string> tokens = DivideStrIntoLexems(command);

    for (std::string & token : tokens)
    {
      if (!subCommand.str().empty())
      {
        subCommand << " ";
      }
      subCommand << token;
      if (CommandConstants::COMMAND_MAP.count(subCommand.str()))
      {
        std::stringstream newFullCommand;
        newFullCommand << CommandConstants::COMMAND_MAP[subCommand.str()]
                       << command.substr(subCommand.str().size(), command.size());
        fullCommand = newFullCommand.str();
        break;
      }
    }
    return fullCommand;
  }
};
