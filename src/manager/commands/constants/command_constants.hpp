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
  static std::string const DIRECTORY_DELIMETR;
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
  static std::string const SVN_TRUNK;
  static std::string const GITHUB_PREFIX;
};
