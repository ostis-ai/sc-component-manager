/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "command_constants.hpp"

std::string const SpecificationConstants::SPECIFICATION_FILENAME = "specification.scs";
std::string const SpecificationConstants::DIRECTORY_DELIMITER = "/";

std::string const GitHubConstants::GIT_CLONE = "git clone";
std::string const GitHubConstants::RAW_GITHUB_PREFIX = "https://raw.githubusercontent.com/";
std::string const GitHubConstants::GITHUB_PREFIX = "https://github.com/";
std::string const GitHubConstants::GITHUB_DOWNLOAD_FILE_COMMAND_PREFIX =
    "curl -H 'Accept: application/vnd.github.v3.raw' -o ";
std::string const GitHubConstants::GITHUB_GET_DEFAULT_BRANCH_COMMAND_PREFIX =
    "curl -s -H \"Accept: application/vnd.github.v3+json\" https://api.github.com/repos/";

std::string const GoogleDriveConstants::GOOGLE_DRIVE_PREFIX = "https://drive.google.com/";
std::string const GoogleDriveConstants::GOOGLE_DRIVE_FILE_PREFIX = "https://drive.google.com/file/d/";
std::string const GoogleDriveConstants::GOOGLE_DRIVE_DOWNLOAD_PREFIX = "https://docs.google.com/uc?export=download&id=";
std::string const GoogleDriveConstants::GOOGLE_DRIVE_POSTFIX = "/view?usp=sharing";
