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
ScAddr ScComponentManagerKeynodes::rrel_repositories;
ScAddr ScComponentManagerKeynodes::rrel_components;
ScAddr ScComponentManagerKeynodes::nrel_authors;
ScAddr ScComponentManagerKeynodes::nrel_explanation;
ScAddr ScComponentManagerKeynodes::nrel_component_address;
ScAddr ScComponentManagerKeynodes::nrel_component_dependencies;
ScAddr ScComponentManagerKeynodes::nrel_installation_method;

}  // namespace keynodes
