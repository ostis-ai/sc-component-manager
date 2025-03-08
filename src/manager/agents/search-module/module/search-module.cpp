/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "search-module.hpp"

#include "agent/sc_component_manager_agent_search.hpp"

using namespace searchModule;

SC_MODULE_REGISTER(SearchModule)->Agent<ScComponentManagerSearchAgent>();
