/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager_command_install.hpp"
#include <sc-memory/utils/sc_exec.hpp>
#include <sc-builder/src/scs_loader.hpp>
#include "src/manager/utils/sc_component_utils.hpp"

#include "src/manager/commands/constants/command_constants.hpp"

ScComponentManagerCommandInstall::ScComponentManagerCommandInstall(std::string specificationsPath)
  : m_specificationsPath(std::move(specificationsPath))
{
}

/**
 * @brief Check if components from specification is available
 * @param context current sc-memory context
 * @param componentsToInstall vector of components identifiers
 * @return vector of available components
 */
ScAddrVector ScComponentManagerCommandInstall::GetAvailableComponents(
    ScMemoryContext * context,
    std::vector<std::string> componentsToInstall)
{
  ScAddrVector availableComponents;
  for (std::string const & componentToInstallIdentifier : componentsToInstall)
  {
    ScAddr componentAddr = context->HelperFindBySystemIdtf(componentToInstallIdentifier);

    SC_LOG_DEBUG("Validating component \"" + componentToInstallIdentifier + "\"");
    try
    {
      ValidateComponent(context, componentAddr);
    }
    catch (utils::ScException const & exception)
    {
      SC_LOG_ERROR("Unable to install component \"" + componentToInstallIdentifier + "\"");
      SC_LOG_DEBUG(exception.Message());
      continue;
    }
    SC_LOG_DEBUG("Component \"" + componentToInstallIdentifier + "\" is specified correctly");
    availableComponents.push_back(componentAddr);
  }
  return availableComponents;
}

/**
 * @brief Installation of component
 * @param context current sc-memory context
 * @param componentAddr component sc-addr
 */
void ScComponentManagerCommandInstall::InstallComponent(ScMemoryContext * context, ScAddr const & componentAddr)
{
  std::vector<std::string> scripts = componentUtils::InstallUtils::GetInstallScripts(context, componentAddr);
  for (auto script : scripts)
  {
    std::string componentDirName =
        componentUtils::InstallUtils::GetComponentDirName(context, componentAddr, m_specificationsPath);
    std::string nodeSystIdtf = context->HelperGetSystemIdtf(componentAddr);
    std::string path = m_specificationsPath + SpecificationConstants::DIRECTORY_DELIMETR + nodeSystIdtf;
    script = "." + script;
    sc_fs_mkdirs(path.c_str());
    ScExec exec{{"cd", path, "&&", script}};
  }
}

ExecutionResult ScComponentManagerCommandInstall::Execute(
    ScMemoryContext * context,
    CommandParameters const & commandParameters)
{
  ExecutionResult executionResult;
  std::vector<std::string> componentsToInstallIdentifiers;
  ScAddrVector componentsToInstall;

  try
  {
    componentsToInstallIdentifiers = commandParameters.at(PARAMETER_NAME);
    componentsToInstall = GetAvailableComponents(context, componentsToInstallIdentifiers);
  }
  catch (std::out_of_range const & exception)
  {
    SC_LOG_INFO("No identifier provided, installing all to install components");
    componentsToInstall = componentUtils::SearchUtils::GetNeedToInstallComponents(context);
  }

  for (ScAddr componentAddr : componentsToInstall)
  {
    InstallDependencies(context, componentAddr);
    DownloadComponent(context, componentAddr);
    InstallComponent(context, componentAddr);
    // TODO: need to process installation method from component specification in kb
  }

  return executionResult;
}

/**
 * @brief Checks if component is valid.
 * Checks if:
 * - component exist;
 * - component is reusable;
 * - component's address link is valid;
 * - component's installation method is valid;
 * Throw exception if failed
 */
void ScComponentManagerCommandInstall::ValidateComponent(ScMemoryContext * context, ScAddr const & componentAddr)
{
  // Check if component exist
  if (!componentAddr.IsValid())
  {
    SC_THROW_EXCEPTION(utils::ExceptionAssert, "Component not found. Unable to install");
  }

  // Check if component is a reusable component
  if (!componentUtils::InstallUtils::IsReusable(context, componentAddr))
  {
    SC_THROW_EXCEPTION(utils::ExceptionAssert, "Component is not a reusable component.");
  }

  // Find and check component address
  if (componentUtils::InstallUtils::GetComponentAddressStr(context, componentAddr).empty())
  {
    SC_THROW_EXCEPTION(utils::ExceptionAssert, "Component address not found.");
  }

  // Find and check component installation method
  if (!componentUtils::InstallUtils::IsComponentInstallationMethodValid(context, componentAddr))
  {
    SC_THROW_EXCEPTION(utils::ExceptionAssert, "Component installation method not found.");
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
  ScAddrVector const & componentDependencies =
      componentUtils::SearchUtils::GetComponentDependencies(context, componentAddr);
  for (ScAddr const & componentDependency : componentDependencies)
  {
    std::string dependencyIdtf = context->HelperGetSystemIdtf(componentDependency);
    SC_LOG_INFO("ScComponentManager: Install dependency \"" + dependencyIdtf + "\"");
    CommandParameters dependencyParameters = {{PARAMETER_NAME, {dependencyIdtf}}};
    ExecutionResult dependencyResult = Execute(context, dependencyParameters);

    // Return empty if you couldn't install one from all dependencies why?
    if (dependencyResult.empty())
    {
      SC_LOG_ERROR("Dependency \"" + dependencyIdtf + "\" is not installed");
      // return dependencyResult;
    }
    else
    {
      result.insert(result.cbegin(), dependencyResult.cbegin(), dependencyResult.cend());
    }
  }

  return result;
}

/**
 * Tries to download component from Github
 */
void ScComponentManagerCommandInstall::DownloadComponent(ScMemoryContext * context, ScAddr const & componentAddr)
{
  downloaderHandler->Download(context, componentAddr);

   std::string componentDirName =
       componentUtils::InstallUtils::GetComponentDirName(context, componentAddr, m_specificationsPath);
   if (!componentUtils::LoadUtils::LoadScsFilesInDir(context, componentDirName))
   {
     SC_LOG_WARNING("Not all files are loaded from " + componentDirName);
   }
}
