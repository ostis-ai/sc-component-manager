/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "ComponentManagerKeynodes.hpp"

#include "sc-memory/sc_memory.hpp"

namespace keynodes
{

ScAddr ComponentManagerKeynodes::concept_reusable_component;
ScAddr ComponentManagerKeynodes::concept_repository;
ScAddr ComponentManagerKeynodes::rrel_repositories;
ScAddr ComponentManagerKeynodes::rrel_components;

}
