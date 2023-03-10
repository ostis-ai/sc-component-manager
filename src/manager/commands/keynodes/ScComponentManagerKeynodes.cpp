/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "ScComponentManagerKeynodes.hpp"

#include "sc-memory/sc_memory.hpp"

namespace keynodes
{
ScAddr ScComponentManagerKeynodes::concept_reusable_component;
ScAddr ScComponentManagerKeynodes::concept_repository;
ScAddr ScComponentManagerKeynodes::concept_complex_address;
ScAddr ScComponentManagerKeynodes::concept_single_address;
ScAddr ScComponentManagerKeynodes::concept_github_url;

ScAddr ScComponentManagerKeynodes::rrel_repositories_specifications;
ScAddr ScComponentManagerKeynodes::rrel_components_specifications;
// ScAddr ScComponentManagerKeynodes::rrel_address;

ScAddr ScComponentManagerKeynodes::nrel_authors;
ScAddr ScComponentManagerKeynodes::nrel_explanation;
ScAddr ScComponentManagerKeynodes::nrel_component_address;
ScAddr ScComponentManagerKeynodes::nrel_component_dependencies;
ScAddr ScComponentManagerKeynodes::nrel_installation_method;
ScAddr ScComponentManagerKeynodes::nrel_alternative_addresses;
ScAddr ScComponentManagerKeynodes::nrel_repository_address;
}  // namespace keynodes
