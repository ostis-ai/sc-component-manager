/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "command_constants.hpp"

std::string const SpecificationConstants::SPECIFICATION_FILENAME = "specification.scs";
std::string const SpecificationConstants::SCS_EXTENSION = ".scs";
char const SpecificationConstants::DIRECTORY_DELIMITER = '/';

std::string const GitHubConstants::GIT_CLONE = "git clone";
std::string const GitHubConstants::RAW_GITHUB_PREFIX = "https://raw.githubusercontent.com/";
std::string const GitHubConstants::GITHUB_PREFIX = "https://github.com/";
std::string const GitHubConstants::TREE = "tree";
std::string const GitHubConstants::CURL_GET_BRANCH_COMMAND =
    "curl -s -H \"Accept: application/vnd.github.v3+json\" https://api.github.com/repos/";
std::string const GitHubConstants::GREP_DEFAULT_BRANCH_COMMAND =
    R"( | grep -w "default_branch" | cut -d ":" -f 2 | tr -d '," ')";
std::string const GitHubConstants::GITHUB_DOWNLOAD_FILE_COMMAND_PREFIX =
    "curl -H 'Accept: application/vnd.github.v3.raw' -o ";
std::string const GitHubConstants::GITHUB_GET_DEFAULT_BRANCH_COMMAND_PREFIX =
    "curl -s -H \"Accept: application/vnd.github.v3+json\" https://api.github.com/repos/";
std::string const GitHubConstants::FLAG_NO_CHECKOUT = "--no-checkout";
std::string const GitHubConstants::FLAG_DEPTH = "--depth=";
std::string const GitHubConstants::FLAG_FILTER_TREE = "--filter=tree:";
std::string const GitHubConstants::GIT_SPARSE_CHECKOUT = "git sparse-checkout set --no-cone";
std::string const GitHubConstants::GIT_CHECKOUT = "git checkout";

std::string const GoogleDriveConstants::GOOGLE_DRIVE_PREFIX = "https://drive.google.com/";
std::string const GoogleDriveConstants::GOOGLE_DRIVE_FILE_PREFIX = "https://drive.google.com/file/d/";
std::string const GoogleDriveConstants::GOOGLE_DRIVE_DOWNLOAD_PREFIX = "https://docs.google.com/uc?export=download&id=";
std::string const GoogleDriveConstants::GOOGLE_DRIVE_POSTFIX = "/view?usp=sharing";

std::map<std::string, std::string> CommandConstants::COMMAND_MAP = {
    {"ci", "components init"},
    {"comp init", "components init"},
    {"cs", "components search"},
    {"comp search", "components search"},
    {"comp s", "components search"},
    {"cinst", "components install"},
    {"comp inst", "components install"}};
std::string const CommandConstants::COMPONENTS_COMMAND_PREFIX = "components";
std::string const CommandConstants::COMPONENTS_COMMAND_INIT = "init";
std::string const CommandConstants::COMPONENTS_COMMAND_INSTALL = "install";
std::string const CommandConstants::COMPONENTS_COMMAND_SEARCH = "search";

std::string const CommandsSearchFlags::IDTF = "idtf";
std::string const CommandsSearchFlags::SET = "set";
std::string const CommandsSearchFlags::EXPLANATION = "explanation";
std::string const CommandsSearchFlags::CLASS = "class";
std::string const CommandsSearchFlags::AUTHOR = "author";
std::string const CommandsSearchFlags::NOTE = "note";
std::string const CommandsSearchFlags::PURPOSE = "purpose";
std::string const CommandsSearchFlags::KEY = "key";
std::string const CommandsSearchFlags::MAIN_ID = "main-id";

std::string const PathKeysOfConfigPath::KB_PATH = "knowledge_base_components_path";
std::string const PathKeysOfConfigPath::PS_PATH = "problem_solver_components_path";
std::string const PathKeysOfConfigPath::UI_PATH = "interface_components_path";
