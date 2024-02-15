/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>
#include <vector>

class SpecificationConstants
{
public:
  static std::string const SPECIFICATION_FILENAME;
  static char const DIRECTORY_DELIMITER;
};

class GoogleDriveConstants
{
public:
  static std::string const GOOGLE_DRIVE_PREFIX;
  static std::string const GOOGLE_DRIVE_FILE_PREFIX;
  static std::string const GOOGLE_DRIVE_DOWNLOAD_PREFIX;
  static std::string const GOOGLE_DRIVE_POSTFIX;
};

class GitHubConstants
{
public:
  static std::string const GIT_CLONE;
  static std::string const FLAG_NO_CHECKOUT;
  static std::string const FLAG_DEPTH;
  static std::string const FLAG_FILTER_TREE;
  static std::string const GIT_SPARSE_CHECKOUT;
  static std::string const GIT_CHECKOUT;
  static std::string const RAW_GITHUB_PREFIX;
  static std::string const GITHUB_PREFIX;
  static std::string const TREE;
  static std::string const CURL_GET_BRANCH_COMMAND;
  static std::string const GREP_DEFAULT_BRANCH_COMMAND;
  static std::string const GITHUB_DOWNLOAD_FILE_COMMAND_PREFIX;
  static std::string const GITHUB_GET_DEFAULT_BRANCH_COMMAND_PREFIX;
};

class CommandConstants
{
public:
  static std::vector<std::string> const COMMAND_COMPONENTS_INIT;
  static std::vector<std::string> const COMMAND_COMPONENTS_SEARCH;
  static std::vector<std::string> const COMMAND_COMPONENTS_INSTALL;
  static std::vector<std::vector<std::string>> const COMMAND_LIST;
  static std::string const COMPONENTS_COMMAND_PREFIX;
};

class CommandsConstantsFlags
{
public:
  static std::string const IDTF;
  static std::string const EXPLANATION;
};
