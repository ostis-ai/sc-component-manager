/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "src/manager/sc_component_manager.hpp"
#include "sc_memory_config.hpp"

class ScComponentManagerFactory
{
public:
  static std::unique_ptr<ScComponentManager> ConfigureScComponentManager(
      ScParams const & scComponentManagerParams,
      sc_memory_params const & memoryParams);
};
