/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

#include "downloader.hpp"

class DownloaderGoogleDrive : public Downloader
{
public:
  bool Download(std::string const & urlAddress, std::string const & downloadPath, std::string const & pathPostfix = "")
      override
  {
    // std::string const gDriveFileId = componentPath.substr(
    //     GoogleDriveConstants::GOOGLE_DRIVE_FILE_PREFIX.size(),
    //     componentPath.size() - GoogleDriveConstants::GOOGLE_DRIVE_FILE_PREFIX.size() -
    //         GoogleDriveConstants::GOOGLE_DRIVE_POSTFIX.size());
    // std::string const gDriveInstallCommandParameter = " -O " + SpecificationConstants::SPECIFICATION_FILENAME;
    // std::string const gDriveInstallCommand = "cd " + specificationsPath + " ; wget --no-check-certificate " + "\'" +
    //                                          GoogleDriveConstants::GOOGLE_DRIVE_DOWNLOAD_PREFIX + gDriveFileId + "\'"
    //                                          + gDriveInstallCommandParameter;

  
    // ScExec exec{{gDriveInstallCommand}};
    return false;
  }
};
