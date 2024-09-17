/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-agents-common/utils/CommonUtils.hpp>
#include "utils/sc_component_utils.hpp"
#include "downloader_handler.hpp"

/**
 * @brief Get class of download node
 * @param context current sc-memory context
 * @param nodeAddr sc-addr of node to download
 * @return Sc-addr of node class if it can be downloaded,
 * return empty sc-addr if class was not found
 */
ScAddr DownloaderHandler::getDownloadableClass(ScMemoryContext * context, ScAddr const & nodeAddr)
{
  ScAddr downloadableClass;

  ScAddrVector const downloadableClasses = {
      keynodes::ScComponentManagerKeynodes::concept_repository,
      keynodes::ScComponentManagerKeynodes::concept_reusable_component,
      keynodes::ScComponentManagerKeynodes::concept_reusable_component_specification};

  for (ScAddr const & currentClass : downloadableClasses)
  {
    if (context->CheckConnector(currentClass, nodeAddr, ScType::EdgeAccessConstPosPerm))
    {
      downloadableClass = currentClass;
    }
  }

  return downloadableClass;
}

/**
 * @brief Get class of address sc-link
 * @param context current sc-memory context
 * @param linkAddr sc-addr of sc-link with address
 * @return Sc-addr of node class in which sc-link is
 * and this class is supported for downloading,
 * return empty sc-addr if class was not found
 */
ScAddr DownloaderHandler::getUrlLinkClass(ScMemoryContext * context, ScAddr const & linkAddr)
{
  ScAddr urlLinkClass;

  ScAddrVector const downloadableUrls = {
      keynodes::ScComponentManagerKeynodes::concept_github_url,
      keynodes::ScComponentManagerKeynodes::concept_google_drive_url};

  for (ScAddr const & currentClass : downloadableUrls)
  {
    if (context->CheckConnector(currentClass, linkAddr, ScType::EdgeAccessConstPosPerm))
    {
      urlLinkClass = currentClass;
    }
  }

  return urlLinkClass;
}

bool DownloaderHandler::DownloadSpecification(ScMemoryContext * context, ScAddr const & componentSpecificationAddr)
{
  ScAddrVector nodeAddressLinkAddrs;

  try
  {
    nodeAddressLinkAddrs = componentUtils::SearchUtils::GetSpecificationAddress(context, componentSpecificationAddr);
  }
  catch (utils::ScException const & exception)
  {
    SC_LOG_ERROR(exception.Message());
    SC_LOG_ERROR(exception.Description());
  }

  std::string urlAddress;
  std::string const & nodeSystemIdentifier = context->GetElementSystemIdentifier(componentSpecificationAddr);
  std::string const & downloadPath = m_downloadDir + SpecificationConstants::DIRECTORY_DELIMITER + nodeSystemIdentifier;
  for (ScAddr const & currentAddressLinkAddr : nodeAddressLinkAddrs)
  {
    ScAddr const & linkAddressClassAddr = getUrlLinkClass(context, currentAddressLinkAddr);
    if (!context->IsElement(linkAddressClassAddr))
    {
      continue;
    }

    if (linkAddressClassAddr == keynodes::ScComponentManagerKeynodes::concept_github_url)
    {
      context->GetLinkContent(currentAddressLinkAddr, urlAddress);
      m_downloader->DownloadFile(downloadPath, urlAddress, SpecificationConstants::SPECIFICATION_FILENAME);
    }
  }

  return true;
}

bool DownloaderHandler::DownloadComponent(ScMemoryContext * context, ScAddr const & componentAddr)
{
  std::string urlAddress;

  ScAddr const & componentAddress = componentUtils::SearchUtils::GetComponentAddress(context, componentAddr);
  context->GetLinkContent(componentAddress, urlAddress);

  m_downloader->DownloadRepository(m_downloadDir, urlAddress);

  return true;
}

void DownloaderHandler::setDownloadDir(std::string const & downloadDir)
{
  m_downloadDir = downloadDir;
}
