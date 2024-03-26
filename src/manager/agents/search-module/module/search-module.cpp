/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "search-module.hpp"

#include "agents/sc_component_manager_agent_search.hpp"

using namespace searchModule;

SC_IMPLEMENT_MODULE(SearchModule)

sc_result SearchModule::InitializeImpl()
{
  if (!SearchModule::InitGlobal())
  {
    SC_LOG_ERROR("SearchModule is deactivated");
    return SC_RESULT_ERROR;
  }

  SC_AGENT_REGISTER(ScComponentManagerSearchAgent);

  return SC_RESULT_OK;
}

sc_result SearchModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(ScComponentManagerSearchAgent);

  return SC_RESULT_OK;
}
