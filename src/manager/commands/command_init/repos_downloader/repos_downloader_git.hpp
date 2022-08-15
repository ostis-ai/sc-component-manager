/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

#include "repos_downloader.hpp"

class ReposDownloaderGit : public ReposDownloader
{
public:
  void Download(std::string const & componentPath, std::string const & specificationsPath) override
  {
    std::string const SPECIFICATION_PATH_POSTFIX = "/trunk/specification.scs";
    std::string const svnInstallCommand = "cd " + specificationsPath + " ; svn export " + componentPath + SPECIFICATION_PATH_POSTFIX;

    system(svnInstallCommand.c_str());
  }

};
