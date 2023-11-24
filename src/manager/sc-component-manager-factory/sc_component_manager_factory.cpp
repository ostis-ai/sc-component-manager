/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager_factory.hpp"
#include "src/manager/sc_component_manager_impl.hpp"

std::unique_ptr<ScComponentManager> ScComponentManagerFactory::ConfigureScComponentManager(
    ScParams const & scComponentManagerParams,
    sc_memory_params const & memoryParams)
{
  std::string const KB_COMPONENT_PATH = "knowledge_base_components_path";
  std::string const PS_COMPONENT_PATH = "problem_solver_components_path";
  std::string const INTERFACE_COMPONENT_PATH = "interface_components_path";

  try
  {
    std::map<ScAddr, std::string, ScAddLessFunc> const & componentsPath = {
        {{keynodes::ScComponentManagerKeynodes::concept_reusable_kb_component,
          scComponentManagerParams.at(KB_COMPONENT_PATH)},
         {keynodes::ScComponentManagerKeynodes::concept_reusable_ps_component,
          scComponentManagerParams.at(PS_COMPONENT_PATH)},
         {keynodes::ScComponentManagerKeynodes::concept_reusable_interface_component,
          scComponentManagerParams.at(INTERFACE_COMPONENT_PATH)}}};

    std::unique_ptr<ScComponentManager> scComponentManager =
        std::unique_ptr<ScComponentManager>(new ScComponentManagerImpl(componentsPath, memoryParams));
    return scComponentManager;
  }
  catch (utils::ScException const & exception)
  {
    SC_LOG_ERROR("ScComponentManagerFacory: ScComponentManager configuration error.");
  }

  return {};
}
