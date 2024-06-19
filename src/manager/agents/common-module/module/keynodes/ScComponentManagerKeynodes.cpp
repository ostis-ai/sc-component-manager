/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "ScComponentManagerKeynodes.hpp"

#include "sc-memory/sc_memory.hpp"

namespace keynodes
{
ScAddr ScComponentManagerKeynodes::myself;
ScAddr ScComponentManagerKeynodes::concept_reusable_component;
ScAddr ScComponentManagerKeynodes::current_components_to_install;
ScAddr ScComponentManagerKeynodes::concept_reusable_kb_component;
ScAddr ScComponentManagerKeynodes::concept_reusable_ps_component;
ScAddr ScComponentManagerKeynodes::concept_reusable_ui_component;
ScAddr ScComponentManagerKeynodes::concept_reusable_embedded_ostis_system;
ScAddr ScComponentManagerKeynodes::concept_repository;
ScAddr ScComponentManagerKeynodes::concept_reusable_component_specification;
ScAddr ScComponentManagerKeynodes::concept_complex_address;
ScAddr ScComponentManagerKeynodes::concept_single_address;
ScAddr ScComponentManagerKeynodes::concept_github_url;
ScAddr ScComponentManagerKeynodes::concept_google_drive_url;
ScAddr ScComponentManagerKeynodes::concept_need_to_install_components;
ScAddr ScComponentManagerKeynodes::sc_model_of_knowledge_base;
ScAddr ScComponentManagerKeynodes::sc_model_of_interface;
ScAddr ScComponentManagerKeynodes::sc_model_of_problem_solver;
ScAddr ScComponentManagerKeynodes::concept_subsystems_set;
ScAddr ScComponentManagerKeynodes::rrel_repositories_specifications;
ScAddr ScComponentManagerKeynodes::rrel_components_specifications;
ScAddr ScComponentManagerKeynodes::nrel_authors;
ScAddr ScComponentManagerKeynodes::nrel_explanation;
ScAddr ScComponentManagerKeynodes::nrel_component_address;
ScAddr ScComponentManagerKeynodes::nrel_component_dependencies;
ScAddr ScComponentManagerKeynodes::nrel_installation_method;
ScAddr ScComponentManagerKeynodes::nrel_alternative_addresses;
ScAddr ScComponentManagerKeynodes::nrel_repository_address;
ScAddr ScComponentManagerKeynodes::nrel_installation_script;
ScAddr ScComponentManagerKeynodes::nrel_decomposition;
ScAddr ScComponentManagerKeynodes::nrel_ostis_system_decomposition;
ScAddr ScComponentManagerKeynodes::action_components_init;
ScAddr ScComponentManagerKeynodes::action_components_search;
ScAddr ScComponentManagerKeynodes::action_components_install;
ScAddr ScComponentManagerKeynodes::rrel_author;
ScAddr ScComponentManagerKeynodes::rrel_sets;
ScAddr ScComponentManagerKeynodes::rrel_components;
ScAddr ScComponentManagerKeynodes::rrel_class;
ScAddr ScComponentManagerKeynodes::rrel_explanation;
ScAddr ScComponentManagerKeynodes::rrel_identifier;
}  // namespace keynodes
