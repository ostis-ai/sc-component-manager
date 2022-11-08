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

#include "sc-memory/sc_scs_helper.hpp"
#include "../sc-builder/src/scs_loader.hpp"

#include "repos_downloader.hpp"
#include "repos_downloader_git.hpp"
#include "repos_downloader_google_drive.hpp"
#include "../../keynodes/ScComponentManagerKeynodes.hpp"

class ReposDownloaderHandler
{
public:
  void HandleComponents(
      ScMemoryContext * context,
      std::string const & componentPath,
      std::string & specificationsPath,
      ScAddr const & componentsSet)
  {
    sc_fs_mkdirs(specificationsPath.c_str());

    for (auto const & it : m_downloaders)
    {
      if (componentPath.find(it.first) != std::string::npos)
      {
        ReposDownloader * downloader = it.second;
        Download(componentPath, specificationsPath, downloader, false);
        break;
      }
    }

    std::string specificationDirName = GetSpecificationDirName(componentPath, specificationsPath);
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
        ReposDownloader * downloader = it.second;
        Download(repositoryPath, specificationsPath, downloader, true);
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
  static char const DIRECTORY_DELIMITER = '/';
  std::map<std::string, ReposDownloader *> m_downloaders = {
      {GitHubConstants::GITHUB_PREFIX, new ReposDownloaderGit()},
      {GoogleDriveConstants::GOOGLE_DRIVE_PREFIX, new ReposDownloaderGoogleDrive()}};

  static void Download(
      std::string const & path,
      std::string & specificationsPath,
      ReposDownloader * downloader,
      bool isRepository)
  {
    std::stringstream specificationDirNameStream;
    std::string specificationDirName = GetSpecificationDirName(path, specificationsPath);
    sc_fs_mkdirs(specificationDirName.c_str());

    downloader->Download(path, specificationDirName, isRepository);
  }

  static std::string GetSpecificationDirName(std::string const & componentPath, std::string & specificationsPath)
  {
    std::stringstream specificationDirNameStream;
    size_t componentDirNameIndex = componentPath.rfind(DIRECTORY_DELIMITER);
    std::string componentDirName = componentPath.substr(componentDirNameIndex);
    specificationDirNameStream << specificationsPath << componentDirName;
    return specificationDirNameStream.str();
  }

  static void AddComponentToSet(
      ScMemoryContext * context,
      std::string const & specificationDirName,
      ScAddr const & componentsSet)
  {
    std::stringstream pathStream;
    pathStream << specificationDirName << DIRECTORY_DELIMITER << SpecificationConstants::SPECIFICATION_FILENAME;

    // TODO: Check if component exists, not load
    ScsLoader loader;
    loader.loadScsFile(*context, pathStream.str());

    ScIterator3Ptr reusableComponentIterator = context->Iterator3(
        keynodes::ScComponentManagerKeynodes::concept_reusable_component,
        ScType::EdgeAccessConstPosPerm,
        ScType::NodeConst);

    if (reusableComponentIterator->Next())
    {
      context->CreateEdge(ScType::EdgeAccessConstPosPerm, componentsSet, reusableComponentIterator->Get(2));
    }
    else  // TODO: check this throw
    {
      SC_THROW_EXCEPTION(
          utils::ExceptionItemNotFound,
          "Incorrect component specification. Reusable component not found in " + specificationDirName);
    }
  }
};
