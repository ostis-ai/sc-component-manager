#pragma once

#include "sc-memory/sc_module.hpp"

#include "init_module.generated.hpp"

namespace initModule
{
class InitModule : public ScModule
{
  SC_CLASS(LoadOrder(100))
  SC_GENERATED_BODY()

  sc_result InitializeImpl() override;

  sc_result ShutdownImpl() override;
};
}
