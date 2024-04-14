/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "common_module.hpp"

#include "keynodes/ScComponentManagerKeynodes.hpp"
#include "utils/common_utils.hpp"

using namespace commonModule;

SC_IMPLEMENT_MODULE(CommonModule)

sc_result CommonModule::InitializeImpl()
{
  if (!CommonModule::InitGlobal())
  {
    SC_LOG_ERROR("CommonModule is deactivated");
    return SC_RESULT_ERROR;
  }

  keynodes::ScComponentManagerKeynodes::InitGlobal();
  common_utils::CommonUtils::InitParametersMap();

  return SC_RESULT_OK;
}

sc_result CommonModule::ShutdownImpl()
{
  return SC_RESULT_OK;
}
