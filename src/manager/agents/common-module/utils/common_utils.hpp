/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "common_utils.generated.hpp"

namespace common_utils
{
class CommonUtils : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  static std::map<std::string, std::string> managerParametersWithAgentRelations;
  static bool TransformToScStruct(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & actionAddr,
    std::map<std::string, std::vector<std::string>> const & commandParameters);
};

}  // namespace keynodes
