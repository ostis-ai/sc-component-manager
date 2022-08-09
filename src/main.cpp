/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <atomic>
#include <thread>

#include "sc-config/sc_config.hpp"
#include "sc_options.hpp"
#include "sc_memory_config.hpp"
#include "sc-memory/utils/sc_signal_handler.hpp"

sc_int main(sc_int argc, sc_char * argv[])
{
  ScOptions options{argc, argv};

  if (options.Has({"help"}))
  {
    std::cout << "SC-COMPONENT-MANAGER USAGE\n\n"
              << "--config|-c -- Path to configuration file\n"
              << "--extensions_path|-e -- Path to directory with sc-memory extensions\n"
              << "--repo_path|-r -- Path to kb.bin folder\n"
              << "--verbose|-v -- Flag to don't save sc-memory state on exit\n"
              << "--clear -- Flag to clear sc-memory on start\n"
              << "--help -- Display this message\n\n";
    return EXIT_SUCCESS;
  }

  std::string configFile;
  if (options.Has({"config", "c"}))
    configFile = options[{"config", "c"}].second;

  ScParams params{options, {}};

  std::vector<std::string> const pathKeys = {"repos_path", "repo_path", "extensions_path", "log_file"};
  ScConfig config{configFile, pathKeys};
  ScConfigGroup configManager = config["sc-component-manager"];
  for (std::string const & key : *configManager)
    params.insert({key, configManager[key]});


  std::vector<std::vector<std::string>> keys
      = {{"extensions_path", "e"}, {"repo_path", "r"}, {"verbose", "v"}, {"clear"}};
  ScParams memoryParams{options, keys};

  // ScMemoryConfig memoryConfig{config, std::move(memoryParams)};

  std::atomic_bool isRun = {SC_TRUE};
  utils::ScSignalHandler::Initialize();
  utils::ScSignalHandler::m_onTerminate = [&isRun]() {
    isRun = SC_FALSE;
  };

  while (isRun)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  return 0;
}
