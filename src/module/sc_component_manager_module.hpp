/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_module.hpp>
#include <sc-config-utils/sc_memory_config.hpp>

#include <console-interface/sc_component_manager.hpp>

class ScComponentManagerModule : public ScModule
{
public:
  void Initialize(ScMemoryContext * context) override;

  void Shutdown(ScMemoryContext * context) override;

protected:
  ScParams m_params;
  ScComponentManager m_scComponentManager;
};
