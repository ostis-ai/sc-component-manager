/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <dirent.h>
#include <sys/stat.h>

#include "src/manager/commands/sc_component_manager_command.hpp"
#include "src/manager/commands/keynodes/ScComponentManagerKeynodes.hpp"
#include "src/manager/downloader/downloader.hpp"
#include "src/manager/downloader/downloader_handler.hpp"

extern "C"
{
#include "sc-core/sc-store/sc-fs-storage/sc_file_system.h"
}

class ScComponentManagerCommandInstall : public ScComponentManagerCommand
{
    std::string const PARAMETER_NAME = "idtf";

public:
    explicit ScComponentManagerCommandInstall(std::string specificationsPath);

    ExecutionResult Execute(ScMemoryContext *context, CommandParameters const &commandParameters) override;

protected:
    static void ValidateComponent(ScMemoryContext *context, ScAddr const &componentAddr);

    void DownloadComponent(ScMemoryContext *context, ScAddr const &componentAddr);

    ExecutionResult InstallDependencies(ScMemoryContext *context, ScAddr const &componentAddr);

    ScAddrVector GetAvailableComponents(ScMemoryContext *context, std::vector <std::string> componentsToInstall);

    void InstallComponent(ScMemoryContext *context, ScAddr const &componentAddr);

    std::string m_specificationsPath;

    std::unique_ptr<DownloaderHandler> downloaderHandler = std::make_unique<DownloaderHandler>(m_specificationsPath);
};
