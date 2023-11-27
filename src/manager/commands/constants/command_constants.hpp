/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

class SpecificationConstants
{
public:
  static std::string const SPECIFICATION_FILENAME;
  static std::string const DIRECTORY_DELIMITER;
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
  static std::string const RAW_GITHUB_PREFIX;
  static std::string const GITHUB_PREFIX;
  static std::string const GITHUB_DOWNLOAD_FILE_COMMAND_PREFIX;
  static std::string const GITHUB_GET_DEFAULT_BRANCH_COMMAND_PREFIX;
};
