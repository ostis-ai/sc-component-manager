/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager_command_install.hpp"

#include "sc-memory/utils/sc_exec.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "src/manager/commands/command_init/constants/command_init_constants.hpp"

ExecutionResult ScComponentManagerCommandInstall::Execute(
    ScMemoryContext * context,
    CommandParameters const & commandParameters)
{
  ExecutionResult result;

  // TODO(MksmOrlov): if no params install all from config (?)
  // try catch at map
  std::vector<std::string> componentsToInstall = commandParameters.at(PARAMETER_NAME);
  for (std::string const & componentToInstall : componentsToInstall)
  {
    ScAddr componentAddr = context->HelperFindBySystemIdtf(componentToInstall);
    if (!componentAddr.IsValid())
    {
      SC_LOG_ERROR("Component \"" + componentToInstall + "\" not found. Unable to install");
      continue;
    }

    // Check if component is a reusable component
    ScIterator3Ptr reusableComponentCLassIterator = context->Iterator3(
        keynodes::ScComponentManagerKeynodes::concept_reusable_component,
        ScType::EdgeAccessConstPosPerm,
        componentAddr);
    if (!reusableComponentCLassIterator->Next())
    {
      SC_LOG_ERROR("Component \"" + componentToInstall + "\" is not a reusable component. Unable to install");
      continue;
    }

    // Find and check component address
    std::string componentAddress = GetComponentAddress(context, componentAddr);
    if (componentAddress.empty())
    {
      SC_LOG_WARNING("Component \"" + componentToInstall + "\" address not found. Unable to install");
      continue;
    }

    // Find and check component installation method
    ScAddr componentInstallationMethod = GetComponentInstallationMethod(context, componentAddr);
    if (!componentInstallationMethod.IsValid())
    {
      SC_LOG_WARNING("Component \"" + componentToInstall + "\" installation method not found. Unable to install");
      continue;
    }

    // Get component dependencies and install them recursively
    ScAddrVector componentDependencies = GetComponentDependencies(context, componentAddr);
    for (ScAddr const & componentDependency : componentDependencies)
    {
      std::string dependencyIdtf = context->HelperGetSystemIdtf(componentDependency);
      SC_LOG_DEBUG("Install dependency \"" + dependencyIdtf + "\"");
      CommandParameters dependencyParameters = {{PARAMETER_NAME, {dependencyIdtf}}};
      ExecutionResult dependencyResult = Execute(context, dependencyParameters);
      // Return empty if you couldn't install dependency
      if (dependencyResult.empty())
      {
        SC_LOG_ERROR("Dependency \"" + dependencyIdtf + "\" is not installed");
        return dependencyResult;
      }
      result.insert(result.cbegin(), dependencyResult.cbegin(), dependencyResult.cend());
    }

    if (componentAddress.find(GitHubConstants::GITHUB_PREFIX) != std::string::npos)
    {
      SC_LOG_WARNING("found github address");
      // TODO(MksmOrlov): get path from config
      ScExec exec{
          {"cd", "/home/mqsm/OSTIS/ostis-web-platform/ims.ostis.kb/ims/library", "&&", "git clone ", componentAddress}};
    }
  }

  return result;
}

std::string ScComponentManagerCommandInstall::GetComponentAddress(
    ScMemoryContext * context,
    ScAddr const & componentAddr)
{
  ScAddr componentAddressAddr;
  std::string componentAddress;
  ScIterator5Ptr componentAddressIterator = context->Iterator5(
      componentAddr,
      ScType::EdgeDCommonConst,
      ScType::LinkConst,
      ScType::EdgeAccessConstPosPerm,
      keynodes::ScComponentManagerKeynodes::nrel_component_address);

  if (componentAddressIterator->Next())
  {
    componentAddressAddr = componentAddressIterator->Get(2);
    componentAddress = utils::CommonUtils::getLinkContent(context, componentAddressAddr);
  }

  return componentAddress;
}

ScAddrVector ScComponentManagerCommandInstall::GetComponentDependencies(
    ScMemoryContext * context,
    ScAddr const & componentAddr)
{
  ScAddrVector componentDependencies;
  ScIterator5Ptr componentDependenciesIterator = context->Iterator5(
      componentAddr,
      ScType::EdgeDCommonConst,
      ScType::NodeConst,
      ScType::EdgeAccessConstPosPerm,
      keynodes::ScComponentManagerKeynodes::nrel_component_dependencies);

  while (componentDependenciesIterator->Next())
  {
    ScAddr componentDependenciesSet = componentDependenciesIterator->Get(2);
    ScAddrVector componentCurrentDependencies =
        utils::IteratorUtils::getAllWithType(context, componentDependenciesSet, ScType::NodeConst);
    componentDependencies.insert(
        componentDependencies.cend(), componentCurrentDependencies.cbegin(), componentCurrentDependencies.cend());
  }

  return componentDependencies;
}

ScAddr ScComponentManagerCommandInstall::GetComponentInstallationMethod(
    ScMemoryContext * context,
    ScAddr const & componentAddr)
{
  ScAddr componentInstallationMethod;
  ScIterator5Ptr componentDependenciesIterator = context->Iterator5(
      componentAddr,
      ScType::EdgeDCommonConst,
      ScType::NodeConst,
      ScType::EdgeAccessConstPosPerm,
      keynodes::ScComponentManagerKeynodes::nrel_installation_method);

  if (componentDependenciesIterator->Next())
    componentInstallationMethod = componentDependenciesIterator->Get(2);

  return componentInstallationMethod;
}

// components install --idtf concept_cat
