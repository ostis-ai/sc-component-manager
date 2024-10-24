/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/CommonUtils.hpp>

#include <sc-memory/sc_agent_context.hpp>

#include "commands/sc_component_manager_command.hpp"

#include "downloader/downloader_handler.hpp"

class ScComponentManagerCommandInit : public ScComponentManagerCommand
{
public:
  explicit ScComponentManagerCommandInit(std::string specificationsPath)
    : m_specificationsPath(std::move(specificationsPath))
  {
  }

  ScAddrUnorderedSet Execute(ScAgentContext * context, ScAddr const & actionAddr) override;

  void ProcessRepository(ScMemoryContext * context, ScAddr & repositories, ScAddrUnorderedSet & specifications);

protected:
  std::string m_specificationsPath;
  std::unique_ptr<DownloaderHandler> downloaderHandler = std::make_unique<DownloaderHandler>(m_specificationsPath);
};
