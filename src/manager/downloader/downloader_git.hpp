/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

#include "sc-memory/utils/sc_exec.hpp"

#include "downloader.hpp"
#include "src/manager/commands/constants/command_constants.hpp"

class DownloaderGit : public Downloader
{
public:
  bool Download(std::string const & downloadPath, std::string const & urlAddress, std::string const & pathPostfix = "")
      override
  {
    std::string path = downloadPath;
    std::stringstream query;
    query << urlAddress << GitHubConstants::SVN_TRUNK << SpecificationConstants::DIRECTORY_DELIMETR << pathPostfix;
    if (pathPostfix.empty())
    {
      size_t const componentDirNameIndex = downloadPath.rfind('/');
      query << "  " << downloadPath.substr(componentDirNameIndex + 1);
      path = downloadPath.substr(0, componentDirNameIndex);
    }

    if (!sc_fs_create_directory(path.c_str()))
    {
      SC_LOG_ERROR("Can't download. Can't create folder.");
      return false;
    }

    ScExec exec{{"cd", path, "&&", "svn", "export", query.str()}};
    return true;
  }
};
