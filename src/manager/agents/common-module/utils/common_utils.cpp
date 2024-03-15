/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "common_utils.hpp"

#include "sc-memory/sc_memory.hpp"

namespace common_utils
{
std::map<std::string, std::string> CommonUtils::managerParametersWithAgentRelations = {
    {"author", "rrel_author"},
    {"class", "rrel_class"},
    {"explanation", "rrel_explanation"},
    {"idtf", "rrel_identifier"}};

bool CommonUtils::TransformToScStruct(
    ScMemoryContext & m_memoryCtx,
    ScAddr const & actionAddr,
    std::map<std::string, std::vector<std::string>> const & commandParameters)
{
  ScAddr value, parameterRRelNode, edge, set;
  for (const auto & parameter : commandParameters)
  {
    try
    {
      parameterRRelNode = m_memoryCtx.HelperFindBySystemIdtf(managerParametersWithAgentRelations.at(parameter.first));
    }
    catch (std::out_of_range const & exception)
    {
      SC_LOG_INFO("ScComponentManagerCommandSearch: Unknown parameter " + parameter.first);
      continue;
    }
    set = m_memoryCtx.CreateNode(ScType::NodeConst);
    edge = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, actionAddr, set);

    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, parameterRRelNode, edge);

    for (const std::string & valueOfParameter : parameter.second)
    {
      if (parameter.first == "explanation")
      {
        value = m_memoryCtx.CreateNode(ScType::LinkConst);
        m_memoryCtx.SetLinkContent(value, valueOfParameter);
      }
      else
      {
        value = m_memoryCtx.HelperFindBySystemIdtf(valueOfParameter);
        if (!value.IsValid())
        {
          value = m_memoryCtx.CreateNode(ScType::NodeConst);
          m_memoryCtx.HelperSetSystemIdtf(valueOfParameter, value);
        }
      }
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, set, value);
    }
  }
  return true;
}

}  // namespace common_utils
