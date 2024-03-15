#include "common_module.hpp"

#include "keynodes/ScComponentManagerKeynodes.hpp"

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

  return SC_RESULT_OK;
}

sc_result CommonModule::ShutdownImpl()
{
  return SC_RESULT_OK;
}
