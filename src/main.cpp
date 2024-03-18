/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <atomic>
#include <thread>

#include "sc-memory/sc_debug.hpp"
#include "sc-config-utils/sc-config/sc_config.hpp"
#include "sc-config-utils/sc_options.hpp"
#include "sc-config-utils/sc_memory_config.hpp"
#include "sc-memory/utils/sc_signal_handler.hpp"

#include "sc_component_manager_impl.hpp"
#include "sc-component-manager-factory/sc_component_manager_factory.hpp"

sc_int main(sc_int argc, sc_char * argv[])
{
  ScOptions options{argc, argv};

  if (options.Has({"help"}))
  {
    std::cout << "SC-COMPONENT-MANAGER USAGE\n\n"
              << "--config|-c -- Path to configuration file\n"
              << "--interactive|-i -- Interactive mode\n"
              << "--extensions_path|-e -- Path to directory with sc-memory extensions\n"
              << "--repo_path|-r -- Path to kb.bin folder\n"
              << "--verbose|-v -- Flag to don't save sc-memory state on exit\n"
              << "--clear -- Flag to clear sc-memory on start\n"
              << "--help -- Display this message\n\n";
    return EXIT_SUCCESS;
  }

  std::string configFilePath;
  if (options.Has({"config", "c"}))
    configFilePath = options[{"config", "c"}].second;

  ScMemory::ms_configPath = configFilePath;

  ScParams params{options, {}};

  ScConfig config{
      configFilePath,
      {"knowledge_base_components_path",
       "problem_solver_components_path",
       "interface_components_path",
       "repo_path",
       "extensions_path",
       "log_file"}};
  ScConfigGroup configManager = config["sc-component-manager"];
  for (std::string const & key : *configManager)
    params.Insert({key, configManager[key]});

  std::vector<std::vector<std::string>> keys = {
      {"extensions_path", "e"}, {"repo_path", "r"}, {"verbose", "v"}, {"clear"}};
  ScParams memoryParams{options, keys};

  ScMemoryConfig memoryConfig{config, memoryParams};

  ScMemory::Initialize(memoryConfig.GetParams());

  common_utils::CommonUtils::InitParametersMap();
  ;
  keynodes::ScComponentManagerKeynodes::InitGlobal();

  std::unique_ptr<ScComponentManager> scComponentManager =
      ScComponentManagerFactory::ConfigureScComponentManager(params, memoryConfig.GetParams());

  std::atomic_bool isRun = {SC_TRUE};
  utils::ScSignalHandler::Initialize();
  utils::ScSignalHandler::m_onTerminate = [&isRun]() {
    isRun = SC_FALSE;
  };

  try
  {
    if (!options.Has({"interactive", "i"}))
    {
      // TODO: Implement interactive mode
      SC_LOG_INFO("Quiet installation mode");
      scComponentManager->QuietInstall();
      return EXIT_SUCCESS;
    }
    scComponentManager->Run();
    SC_LOG_INFO("ScComponentManager started");
    while (isRun)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    scComponentManager->Stop();
  }
  catch (utils::ScException const & exception)
  {
    SC_LOG_ERROR(exception.Description());
    scComponentManager->Stop();

    return EXIT_FAILURE;
  }
  SC_LOG_INFO("ScComponentManager finished");

  return EXIT_SUCCESS;
}
