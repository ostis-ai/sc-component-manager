/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_type.hpp>
#include <sc-memory/sc_iterator.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "src/manager/commands/keynodes/ScComponentManagerKeynodes.hpp"
#include "sc_component_utils.hpp"

namespace componentUtils
{

/**
 * @brief Get content of component address link content
 * @param context current sc-memory context
 * @param componentAddr sc-addr of the component to install
 * @return sc-addr of component address
 */
ScAddr SearchUtils::GetComponentAddress(ScMemoryContext * context, ScAddr const & componentAddr)
{
  ScAddr componentAddressAddr;
  ScIterator5Ptr const & componentAddressIterator = context->Iterator5(
      componentAddr,
      ScType::EdgeDCommonConst,
      ScType::LinkConst,
      ScType::EdgeAccessConstPosPerm,
      keynodes::ScComponentManagerKeynodes::nrel_component_address);

  if (componentAddressIterator->Next())
  {
    componentAddressAddr = componentAddressIterator->Get(2);
  }

  return componentAddressAddr;
}

/**
 * @brief Get component dependencies vector
 * @param context current sc-memory context
 * @param componentAddr sc-addr of the component to install
 * @return ScAddrVector consists of component dependencies sc-addrs,
 * return empty vector if component has no dependencies
 */
ScAddrVector SearchUtils::GetComponentDependencies(ScMemoryContext * context, ScAddr const & componentAddr)
{
  ScAddrVector componentDependencies;
  ScAddr componentDependenciesSet;
  ScAddrVector componentCurrentDependencies;

  ScIterator5Ptr const & componentDependenciesIterator = context->Iterator5(
      componentAddr,
      ScType::EdgeDCommonConst,
      ScType::NodeConst,
      ScType::EdgeAccessConstPosPerm,
      keynodes::ScComponentManagerKeynodes::nrel_component_dependencies);

  while (componentDependenciesIterator->Next())
  {
    componentDependenciesSet = componentDependenciesIterator->Get(2);
    componentCurrentDependencies =
        utils::IteratorUtils::getAllWithType(context, componentDependenciesSet, ScType::NodeConst);
    componentDependencies.insert(
        componentDependencies.cend(), componentCurrentDependencies.cbegin(), componentCurrentDependencies.cend());
  }

  return componentDependencies;
}

/**
 * @brief Get component installation method
 * @param context current sc-memory context
 * @param componentAddr sc-addr of the component to install
 * @return sc-addr of the component installation method
 */
ScAddr SearchUtils::GetComponentInstallationMethod(ScMemoryContext * context, ScAddr const & componentAddr)
{
  ScAddr componentInstallationMethod;
  ScIterator5Ptr const & componentDependenciesIterator = context->Iterator5(
      componentAddr,
      ScType::EdgeDCommonConst,
      ScType::NodeConst,
      ScType::EdgeAccessConstPosPerm,
      keynodes::ScComponentManagerKeynodes::nrel_installation_method);

  if (componentDependenciesIterator->Next())
  {
    componentInstallationMethod = componentDependenciesIterator->Get(2);
  }

  return componentInstallationMethod;
}

}  // namespace componentUtils
