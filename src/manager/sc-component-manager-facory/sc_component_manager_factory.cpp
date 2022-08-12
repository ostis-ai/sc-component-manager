/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager_factory.hpp"
#include "src/manager/sc_component_manager_impl.hpp"

std::unique_ptr<ScComponentManager> ScComponentManagerFactory::ConfigureScComponentManager(
    const ScParams & scComponentManagerParams,
    sc_memory_params memoryParams)
{
  std::unique_ptr<ScComponentManager> scComponentManager = std::unique_ptr<ScComponentManager>(
      new ScComponentManagerImpl(scComponentManagerParams.at("repos_path"), memoryParams));

  return scComponentManager;
}
