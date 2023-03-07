/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_memory.hpp>

namespace componentUtils
{

class SearchUtils
{
public:
  static ScAddr GetComponentAddress(ScMemoryContext * context, ScAddr const & componentAddr);

  static ScAddrVector GetComponentDependencies(ScMemoryContext * context, ScAddr const & componentAddr);

  static ScAddr GetComponentInstallationMethod(ScMemoryContext * context, ScAddr const & componentAddr);
};

}  // namespace componentUtils
