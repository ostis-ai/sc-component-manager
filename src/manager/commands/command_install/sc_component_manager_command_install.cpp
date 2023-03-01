/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager_command_install.hpp"

#include "sc-memory/utils/sc_exec.hpp"
#include "../sc-builder/src/scs_loader.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "src/manager/commands/command_init/constants/command_init_constants.hpp"

ScComponentManagerCommandInstall::ScComponentManagerCommandInstall(std::string specificationsPath)
  : m_specificationsPath(std::move(specificationsPath))
{
}

ExecutionResult ScComponentManagerCommandInstall::Execute(
    ScMemoryContext * context,
    CommandParameters const & commandParameters)
{
  ExecutionResult result;
  std::vector<std::string> componentsToInstall;

  try
  {
    componentsToInstall = commandParameters.at(PARAMETER_NAME);
  }
  catch (std::exception & ex)
  {
    SC_LOG_INFO("No identifier provided, installing all to install components");

    return result;
  }

  componentsToInstall = commandParameters.at(PARAMETER_NAME);
  for (std::string const & componentToInstallIdentifier : componentsToInstall)
  {
    ScAddr componentAddr = context->HelperFindBySystemIdtf(componentToInstallIdentifier);

    if (!componentAddr.IsValid())
    {
      SC_LOG_ERROR("Component \"" + componentToInstallIdentifier + "\" not found. Unable to install");
      continue;
    }

    SC_LOG_INFO("Checking component \"" + componentToInstallIdentifier);
    if (!CheckComponent(context, componentAddr))
    {
      SC_LOG_WARNING("Unable to install component \"" + componentToInstallIdentifier);
      continue;
    }
    SC_LOG_INFO("Component \"" + componentToInstallIdentifier + "\" checked successfully");

    InstallDependencies(context, componentAddr);

    DownloadComponent(context, componentAddr);

    // Interpret installation method
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


/**
* Get component dependencies vector
* @param context current sc-memory context
* @param componentAddr sc-addr of component
* @return ScAddrVector consists of component dependencies sc-addrs,
* will be empty if component has no dependencies
*/
ScAddrVector ScComponentManagerCommandInstall::GetComponentDependencies(
    ScMemoryContext * context,
    ScAddr const & componentAddr)
{
  ScAddrVector componentDependencies;
  ScIterator5Ptr const componentDependenciesIterator = context->Iterator5(
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


/**
 * Get component installation method
 * @return sc-addr of component installation method
*/
ScAddr ScComponentManagerCommandInstall::GetComponentInstallationMethod(
    ScMemoryContext * context,
    ScAddr const & componentAddr)
{
  ScAddr componentInstallationMethod;
  ScIterator5Ptr const componentDependenciesIterator = context->Iterator5(
      componentAddr,
      ScType::EdgeDCommonConst,
      ScType::NodeConst,
      ScType::EdgeAccessConstPosPerm,
      keynodes::ScComponentManagerKeynodes::nrel_installation_method);

  if (componentDependenciesIterator->Next())
    componentInstallationMethod = componentDependenciesIterator->Get(2);

  return componentInstallationMethod;
}

/**
 * Checks if component is valid.
 * Checks if:
 * - component is reusable;
 * - component's address is valid;
 * - component's installation method is valid;
 * @return Returns true if component is valid.
 */
bool ScComponentManagerCommandInstall::CheckComponent(ScMemoryContext * context, ScAddr const & componentAddr)
{
  // Check if component is a reusable component
  ScIterator3Ptr const reusableComponentCLassIterator = context->Iterator3(
      keynodes::ScComponentManagerKeynodes::concept_reusable_component, ScType::EdgeAccessConstPosPerm, componentAddr);
  if (!reusableComponentCLassIterator->Next())
  {
    SC_LOG_WARNING("Component is not a reusable component. Unable to install");
    return false;
  }

  // Find and check component address
  std::string const componentAddress = GetComponentAddress(context, componentAddr);
  if (componentAddress.empty())
  {
    SC_LOG_WARNING("Component address not found. Unable to install");
    return false;
  }

  // Find and check component installation method
  ScAddr const & componentInstallationMethod = GetComponentInstallationMethod(context, componentAddr);
  if (!componentInstallationMethod.IsValid())
  {
    SC_LOG_WARNING("Component installation method not found. Unable to install");
    return false;
  }
}

/**
 * Tries to install component dependencies.
 * @return Returns {DependencyIdtf1, DependencyIdtf2, ...}
 * if installation successfull, otherwise
 * returns empty vector.
 */
ExecutionResult ScComponentManagerCommandInstall::InstallDependencies(
    ScMemoryContext * context,
    ScAddr const & componentAddr)
{
  ExecutionResult result;
  // Get component dependencies and install them recursively
  ScAddrVector componentDependencies = GetComponentDependencies(context, componentAddr);
  for (ScAddr const & componentDependency : componentDependencies)
  {
    std::string dependencyIdtf = context->HelperGetSystemIdtf(componentDependency);
    SC_LOG_INFO("ScComponentManager: Install dependency \"" + dependencyIdtf + "\"");
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

  return result;
}

/**
* Tries to download component from Github
*/
void ScComponentManagerCommandInstall::DownloadComponent(ScMemoryContext * context, ScAddr const & componentAddr)
{

  std::string componentAddress = GetComponentAddress(context, componentAddr);

  if (componentAddress.find(GitHubConstants::GITHUB_PREFIX) != std::string::npos)
  {
    struct stat sb
    {
    };
    size_t componentDirNameIndex = componentAddress.rfind('/');
    std::string componentDirName = m_specificationsPath + componentAddress.substr(componentDirNameIndex);
    while (stat(componentDirName.c_str(), &sb) == 0)
    {
      componentDirName += componentAddress.substr(componentDirNameIndex);
    }
    sc_fs_mkdirs(componentDirName.c_str());

    ScExec exec{{"cd", componentDirName, "&&", "git clone ", componentAddress}};

    ScsLoader loader;
    DIR * dir;
    struct dirent * diread;
    componentDirName += componentAddress.substr(componentDirNameIndex);
    if ((dir = opendir(componentDirName.c_str())) != nullptr)
    {
      while ((diread = readdir(dir)) != nullptr)
      {
        std::string filename = diread->d_name;
        if (filename.rfind(".scs") != std::string::npos)
        {
          loader.loadScsFile(*context, componentDirName + "/" + filename);
        }
      }
      closedir(dir);
    }
  }
}
