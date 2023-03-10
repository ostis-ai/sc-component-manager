/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "repos_downloader_handler.hpp"
#include "../sc-builder/src/scs_loader.hpp"

// void ReposDownloaderHandler::HandleComponents(
//     ScMemoryContext * context,
//     std::string const & componentPath,
//     std::string & specificationsPath)
// {
//   sc_fs_mkdirs(specificationsPath.c_str());

//   for (auto const & it : m_downloaders)
//   {
//     if (componentPath.find(it.first) != std::string::npos)
//     {
//       ReposDownloader * downloader = it.second;
//       Download(componentPath, specificationsPath, downloader, false);
//       break;
//     }
//   }

//   std::string specificationDirName = GetSpecificationDirName(componentPath, specificationsPath);
//   ScsLoader loader;
//   std::stringstream pathStream;
//   pathStream << specificationDirName << DIRECTORY_DELIMITER << SpecificationConstants::SPECIFICATION_FILENAME;
//   loader.loadScsFile(*context, pathStream.str());
// }

// void ReposDownloaderHandler::HandleRepositories(std::string const & repositoryPath, std::string & specificationsPath)
// {
//   sc_fs_mkdirs(specificationsPath.c_str());

//   std::stringstream specificationDirName;

//   for (auto const & it : m_downloaders)
//   {
//     if (repositoryPath.find(it.first) != std::string::npos)
//     {
//       ReposDownloader * downloader = it.second;
//       Download(repositoryPath, specificationsPath, downloader, true);
//       specificationsPath = specificationDirName.str();
//       break;
//     }
//   }
// }

// ReposDownloaderHandler::~ReposDownloaderHandler()
// {
//   for (auto const & it : m_downloaders)
//     delete it.second;
// }

// void ReposDownloaderHandler::Download(
//     std::string const & path,
//     std::string & specificationsPath,
//     ReposDownloader * downloader,
//     bool isRepository)
// {
//   std::stringstream specificationDirNameStream;
//   std::string specificationDirName = GetSpecificationDirName(path, specificationsPath);
//   sc_fs_mkdirs(specificationDirName.c_str());

//   downloader->Download(path, specificationDirName, isRepository);
// }

void ReposDownloaderHandler::Download(ScMemoryContext * context, ScAddr const & addressLinkAddr)
{
  
}

// std::string ReposDownloaderHandler::GetSpecificationDirName(
//     std::string const & componentPath,
//     std::string & specificationsPath)
// {
//   std::stringstream specificationDirNameStream;
//   size_t componentDirNameIndex = componentPath.rfind(DIRECTORY_DELIMITER);
//   std::string componentDirName = componentPath.substr(componentDirNameIndex);
//   specificationDirNameStream << specificationsPath << componentDirName;
//   return specificationDirNameStream.str();
// }
