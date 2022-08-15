/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>
#include <map>

extern "C"
{
#include "sc-core/sc-store/sc-fs-storage/sc_file_system.h"
}

#include "repos_downloader.hpp"
#include "repos_downloader_git.hpp"
#include "repos_downloader_google_drive.hpp"

class ReposDownloaderHandler
{
public:
  void Handle(std::string const & componentPath, std::string const & specificationsPath)
  {
    sc_fs_mkdirs(specificationsPath.c_str());

    std::string componentDirName;
    size_t componentDirNameIndex;
    std::stringstream specificationDirName;

    for (auto const & it : m_downloaders)
    {
      if (componentPath.find(it.first) != std::string::npos)
      {
        componentDirNameIndex = componentPath.rfind('/');
        componentDirName = componentPath.substr(componentDirNameIndex);
        specificationDirName << specificationsPath << componentDirName;

        sc_fs_mkdirs(specificationDirName.str().c_str());

        ReposDownloader * downloader = it.second;
        downloader->Download(componentPath, specificationDirName.str());
        break;
      }
    }

  }

  ~ReposDownloaderHandler()
  {
    for (auto const & it : m_downloaders)
      delete it.second;
  }

protected:
  std::map<std::string, ReposDownloader *> m_downloaders = {
    {"https://github.com/", new ReposDownloaderGit()},
    {"https://drive.google.com/", new ReposDownloaderGoogleDrive()}
  };
};
