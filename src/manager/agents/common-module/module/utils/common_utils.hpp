/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

namespace common_utils
{
class CommonUtils
{
public:
  static std::map<std::string, ScAddr> managerParametersWithAgentRelations;

  static void InitParametersMap();

  static bool TransformToScStruct(
      ScMemoryContext & m_memoryCtx,
      ScAddr const & actionAddr,
      std::map<std::string, std::vector<std::string>> const & commandParameters);

  static ScAddrVector GetNodesUnderParameter(
      ScMemoryContext & m_memoryCtx,
      ScAddr const & actionAddr,
      ScAddr const & relationAddr);

  static ScAddr GetParameterNodeUnderRelation(
      ScMemoryContext & m_memoryCtx,
      ScAddr const & actionAddr,
      ScAddr const & relation);

  static std::map<std::string, ScAddr> GetSetElements(ScMemoryContext & m_memoryCtx, ScAddr const & setAddr);

  static std::map<std::string, ScAddr> GetElementsLinksOfSet(ScMemoryContext & m_memoryCtx, ScAddr const & setAddr);

  static std::map<std::string, std::vector<std::string>> GetCommandParameters(
      ScMemoryContext & m_memoryCtx,
      ScAddr const & actionAddr);
};
}  // namespace common_utils
