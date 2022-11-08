/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "command_init_constants.hpp"

std::string const SpecificationConstants::SPECIFICATION_FILENAME = "specification.scs";
std::string const SpecificationConstants::REPOS_FILENAME = "repos.txt";
std::string const SpecificationConstants::REPOS_SECTION_HEADER = "[repositories]";
std::string const SpecificationConstants::COMPONENTS_SECTION_HEADER = "[components]";

std::string const GitHubConstants::SVN_TRUNK = "/trunk";
std::string const GitHubConstants::GITHUB_PREFIX = "https://github.com/";

std::string const GoogleDriveConstants::GOOGLE_DRIVE_PREFIX = "https://drive.google.com/";
std::string const GoogleDriveConstants::GOOGLE_DRIVE_FILE_PREFIX = "https://drive.google.com/file/d/";
std::string const GoogleDriveConstants::GOOGLE_DRIVE_DOWNLOAD_PREFIX = "https://docs.google.com/uc?export=download&id=";
std::string const GoogleDriveConstants::GOOGLE_DRIVE_POSTFIX = "/view?usp=sharing";
