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
#include "src/manager/commands/keynodes/ComponentManagerKeynodes.hpp"

#include "sc-memory/sc_scs_helper.hpp"
#include "../sc-builder/src/scs_loader.hpp"

class ReposDownloaderHandler
{
public:
  void HandleComponents(ScMemoryContext * context, std::string const & componentPath, std::string & specificationsPath, ScAddr const & componentsSet)
  {
    sc_fs_mkdirs(specificationsPath.c_str());

    std::stringstream specificationDirNameStream;
    std::string specificationDirName;

    for (auto const & it : m_downloaders)
    {
      if (componentPath.find(it.first) != std::string::npos)
      {
        size_t componentDirNameIndex = componentPath.rfind('/');
        std::string componentDirName = componentPath.substr(componentDirNameIndex);
        specificationDirNameStream << specificationsPath << componentDirName;
        specificationDirName = specificationDirNameStream.str();
        sc_fs_mkdirs(specificationDirName.c_str());

        ReposDownloader * downloader = it.second;
        downloader->Download(componentPath, specificationDirName, false);
        break;
      }
    }

    AddComponentToSet(context, specificationDirName, componentsSet);
  }

  void HandleRepositories(std::string const & repositoryPath, std::string & specificationsPath)
  {
    sc_fs_mkdirs(specificationsPath.c_str());

    std::stringstream specificationDirName;

    for (auto const & it : m_downloaders)
    {
      if (repositoryPath.find(it.first) != std::string::npos)
      {
        size_t repositoryDirNameIndex = repositoryPath.rfind('/');
        std::string repositoryDirName = repositoryPath.substr(repositoryDirNameIndex);
        specificationDirName << specificationsPath << repositoryDirName;

        sc_fs_mkdirs(specificationDirName.str().c_str());

        ReposDownloader * downloader = it.second;
        downloader->Download(repositoryPath, specificationDirName.str(), true);
        specificationsPath = specificationDirName.str();
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
    {GitHubConstants::GITHUB_PREFIX, new ReposDownloaderGit()},
    {GoogleDriveConstants::GOOGLE_DRIVE_PREFIX, new ReposDownloaderGoogleDrive()}
  };

  static void AddComponentToSet(ScMemoryContext * context, std::string const & specificationDirName, ScAddr const & componentsSet)
  {
    std::stringstream pathStream;
    pathStream << specificationDirName << '/' << SpecificationConstants::SPECIFICATION_FILENAME;

    ScsLoader loader;
    loader.loadScsFile(*context, pathStream.str());

    ScIterator3Ptr reusableComponentIterator = context->Iterator3(
        keynodes::ComponentManagerKeynodes::concept_reusable_component,
        ScType::EdgeAccessConstPosPerm,
        ScType::NodeConst);

    if (reusableComponentIterator->Next())
    {
      context->CreateEdge(ScType::EdgeAccessConstPosPerm, componentsSet, reusableComponentIterator->Get(2));
    }
    else // TODO: check this throw
    {
      SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "Incorrect component specification. Reusable component not found in " + specificationDirName);
    }
  }
};
