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

ScComponentManagerCommandInstall::ScComponentManagerCommandInstall(std::map<ScAddr, std::string, ScAddrLessFunc> componentsPath)
  : m_componentsPath(std::move(componentsPath))
{
  downloaderHandler = std::make_unique<DownloaderHandler>();
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

    SC_LOG_DEBUG("ScComponentManagerCommandInstall: Validating component \"" + componentToInstallIdentifier + "\"");
    try
    {
      ValidateComponent(context, componentAddr);
    }
    catch (utils::ScException const & exception)
    {
      SC_LOG_ERROR(
          "ScComponentManagerCommandInstall: Unable to install component \"" + componentToInstallIdentifier + "\"");
      SC_LOG_DEBUG(exception.Message());
      continue;
    }
    SC_LOG_DEBUG(
        "ScComponentManagerCommandInstall: Component \"" + componentToInstallIdentifier + "\" is specified correctly");
    availableComponents.push_back(componentAddr);
  }
  return availableComponents;
}

/**
 * @brief Installation of component
 * @param context current sc-memory context
 * @param componentAddr component sc-addr
 */
bool ScComponentManagerCommandInstall::InstallComponent(ScMemoryContext * context, ScAddr const & componentAddr)
{
  std::vector<std::string> scripts = componentUtils::InstallUtils::GetInstallScripts(context, componentAddr);

  std::stringstream path;
  for (std::string script : scripts)
  {
    std::string componentDirName =
        componentUtils::InstallUtils::GetComponentDirName(context, componentAddr, m_downloadDir);
    std::string nodeSystIdtf = context->HelperGetSystemIdtf(componentAddr);
    path << m_downloadDir << SpecificationConstants::DIRECTORY_DELIMETR << nodeSystIdtf;
    script = "." + script;
    sc_fs_create_directory(path.str().c_str());
    ScExec exec{{"cd", path.str(), "&&", script}};
  }

  return true;
}

bool ScComponentManagerCommandInstall::Execute(ScMemoryContext * context, CommandParameters const & commandParameters)
{
  bool executionResult = true;
  std::vector<std::string> componentsToInstallIdentifiers;
  ScAddrVector componentsToInstall;

  try
  {
    componentsToInstallIdentifiers = commandParameters.at(PARAMETER_NAME);
    componentsToInstall = GetAvailableComponents(context, componentsToInstallIdentifiers);
  }
  catch (std::out_of_range const & exception)
  {
    SC_LOG_INFO("ScComponentManagerCommandInstall: No identifier provided, installing all to install components");
    componentsToInstall = componentUtils::SearchUtils::GetNeedToInstallComponents(context);
  }

  for (ScAddr componentAddr : componentsToInstall)
  {
    executionResult = InstallDependencies(context, componentAddr);
    executionResult &= DownloadComponent(context, componentAddr);
    executionResult &= InstallComponent(context, componentAddr);
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
    SC_THROW_EXCEPTION(
        utils::ExceptionAssert, "ScComponentManagerCommandInstall: Component not found. Unable to install");
  }

  // Check if component is a reusable component
  if (!componentUtils::InstallUtils::IsReusable(context, componentAddr))
  {
    SC_THROW_EXCEPTION(
        utils::ExceptionAssert, "ScComponentManagerCommandInstall: Component is not a reusable component.");
  }

  // Find and check component address
  if (componentUtils::InstallUtils::GetComponentAddressStr(context, componentAddr).empty())
  {
    SC_THROW_EXCEPTION(utils::ExceptionAssert, "ScComponentManagerCommandInstall: Component address not found.");
  }

  // Find and check component installation method
  if (!componentUtils::InstallUtils::IsComponentInstallationMethodValid(context, componentAddr))
  {
    SC_THROW_EXCEPTION(
        utils::ExceptionAssert, "ScComponentManagerCommandInstall: Component installation method not found.");
  }
}

/**
 * Tries to install component dependencies.
 * @return Returns {DependencyIdtf1, DependencyIdtf2, ...}
 * if installation successfull, otherwise
 * returns empty vector.
 */
bool ScComponentManagerCommandInstall::InstallDependencies(ScMemoryContext * context, ScAddr const & componentAddr)
{
  bool result = true;
  // Get component dependencies and install them recursively
  ScAddrVector const & componentDependencies =
      componentUtils::SearchUtils::GetComponentDependencies(context, componentAddr);
  for (ScAddr const & componentDependency : componentDependencies)
  {
    std::string dependencyIdtf = context->HelperGetSystemIdtf(componentDependency);
    SC_LOG_INFO("ScComponentManager: Install dependency \"" + dependencyIdtf + "\"");
    CommandParameters dependencyParameters = {{PARAMETER_NAME, {dependencyIdtf}}};
    bool dependencyResult = Execute(context, dependencyParameters);

    // Return empty if you couldn't install one from all dependencies why?
    if (!dependencyResult)
    {
      SC_LOG_ERROR("ScComponentManagerCommandInstall: Dependency \"" + dependencyIdtf + "\" is not installed");
      // return dependencyResult;
      result = false;
    }
  }

  return result;
}

/**
 * Tries to download component from Github
 */
bool ScComponentManagerCommandInstall::DownloadComponent(ScMemoryContext * context, ScAddr const & componentAddr)
{
  ScAddr componentClass;
  ScIterator3Ptr const & componentClassIterator = context->Iterator3(ScType::NodeConstClass, ScType::EdgeAccessConstPosPerm, componentAddr);
  while (componentClassIterator->Next())
  {
    componentClass = componentClassIterator->Get(0);
    if (m_componentsPath.count(componentClass) != 0)
    {
      m_downloadDir = m_componentsPath.at(componentClass);
      break;
    }
  }

  downloaderHandler->setDownloadDir(m_downloadDir);
  bool result = downloaderHandler->Download(context, componentAddr);
  std::string componentDirName =
      componentUtils::InstallUtils::GetComponentDirName(context, componentAddr, m_downloadDir);

  if (!componentUtils::LoadUtils::LoadScsFilesInDir(context, componentDirName))
  {
    SC_LOG_WARNING("ScComponentManagerCommandInstall: Not all files are loaded from " + componentDirName);
  }

  return result;
}
