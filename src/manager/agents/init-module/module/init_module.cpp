/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "init_module.hpp"

#include "agent/sc_component_manager_agent_init.hpp"

using namespace initModule;

SC_MODULE_REGISTER(InitModule)->Agent<ScComponentManagerInitAgent>();
