/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

#include "repos_downloader.hpp"

class ReposDownloaderGoogleDrive : public ReposDownloader
{
public:
  void Download(std::string const & componentPath, std::string const & specificationsPath) override
  {
    std::string const gDrivePrefix = "https://drive.google.com/file/d/";
    std::string const gDriveDownloadPrefix = "https://docs.google.com/uc?export=download&id=";
    std::string const gDrivePostfix = "/view?usp=sharing";
    std::string const gDriveFileId = componentPath.substr(gDrivePrefix.size(), componentPath.size() - gDrivePrefix.size() - gDrivePostfix.size());
    std::string const gDriveInstallCommandParameter = " -O specification.scs";
    std::string const gDriveInstallCommand = "cd " + specificationsPath + " ; wget --no-check-certificate " + "\'" + gDriveDownloadPrefix + gDriveFileId + "\'" + gDriveInstallCommandParameter;

    system(gDriveInstallCommand.c_str());
  }

};
