/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_module.hpp>

#include "sc_component_manager.hpp"

#include "sc_component_manager_module.generated.hpp"

class ScComponentManagerModule : public ScModule
{
  SC_CLASS(LoadOrder(200))
  SC_GENERATED_BODY()

  sc_result InitializeImpl() override;

  sc_result ShutdownImpl() override;

protected:
  ScParams m_params;
  std::unique_ptr<ScComponentManager> m_scComponentManager;
};
