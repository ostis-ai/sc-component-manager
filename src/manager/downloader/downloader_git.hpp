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
    SC_LOG_WARNING("downloadPath = " << downloadPath);
    SC_LOG_WARNING("urlAddress = " << urlAddress);
    SC_LOG_WARNING("pathPostfix = " << pathPostfix);

    std::string path = downloadPath;
    std::stringstream query;
    if (pathPostfix.empty())
    {
      SC_LOG_WARNING("pathPostfix empty");
      size_t const componentDirNameIndex = downloadPath.rfind('/');
      query << "  " << downloadPath.substr(componentDirNameIndex + 1);
      path = downloadPath.substr(0, componentDirNameIndex);
    }
    std::string componentGithubPath;
    size_t position = urlAddress.find(GitHubConstants::GITHUB_PREFIX) + GitHubConstants::GITHUB_PREFIX.size();
    componentGithubPath = urlAddress.substr(position);
    SC_LOG_WARNING(componentGithubPath);
    query << GitHubConstants::GITHUB_CURL_COMMAND << path << SpecificationConstants::DIRECTORY_DELIMETR <<
      pathPostfix << " " << GitHubConstants::RAW_GITHUB_PREFIX << componentGithubPath <<
      SpecificationConstants::DIRECTORY_DELIMETR << "main" << SpecificationConstants::DIRECTORY_DELIMETR << pathPostfix;

    if (!sc_fs_create_directory(path.c_str()))
    {
      SC_LOG_ERROR("Can't download. Can't create folder. " << path);
      return false;
    }

    SC_LOG_WARNING(query.str());
    ScExec exec{{query.str()}};
    return true;
  }
};

/*
 curl -H 'Accept: application/vnd.github.v3.raw' -o /home/mqsm/OSTIS/temp.txt https://raw.githubusercontent.com/ostis-ai/ims.ostis.kb/main/specification.scs

 curl -s -H "Accept: application/vnd.github.v3+json" \
  https://api.github.com/repos/MksmOrlov/cat-kb-component \
  | grep -w "default_branch" \
  | cut -d ":" -f 2 \
  | tr -d '," '
 */