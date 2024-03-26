/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager_command_install.hpp"
#include <sc-memory/utils/sc_exec.hpp>
#include "utils/sc_component_utils.hpp"

#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "../../agents/common-module/module/utils/common_utils.hpp"

ScComponentManagerCommandInstall::ScComponentManagerCommandInstall(
    std::map<ScAddr, std::string, ScAddrLessFunc> componentsPath)
  : m_componentsPath(std::move(componentsPath))
{
  downloaderHandler = std::make_unique<DownloaderHandler>(m_downloadDir);
}

/**
 * @brief Check if components from specification is available
 * @param context current sc-memory context
 * @param componentsToInstall vector of components identifiers
 * @return vector of available components
 */
ScAddrVector ScComponentManagerCommandInstall::GetAvailableComponents(
    ScMemoryContext * context,
    std::vector<std::string> const & componentsToInstallIdentifiers)
{
  ScAddrVector availableComponents;
  ScAddr componentAddr;
  for (std::string const & componentToInstallIdentifier : componentsToInstallIdentifiers)
  {
    try
    {
      componentAddr = context->HelperFindBySystemIdtf(componentToInstallIdentifier);

      SC_LOG_DEBUG("ScComponentManagerCommandInstall: Validating component \"" + componentToInstallIdentifier + "\"");

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

  // TODO: remove cycle after removing the init-command
  std::set<std::string> setOfScripts;
  for (std::string const & script : scripts)
  {
    setOfScripts.insert(script);
  }

  for (std::string const & script : setOfScripts)
  {
    std::string const & componentDirName =
        componentUtils::InstallUtils::GetComponentDirName(context, componentAddr, m_downloadDir);

    std::stringstream command;
    command << "cd " << componentDirName << " && "
            << "." << script;

    ScExec Exec({command.str()});
  }

  return true;
}

ScAddrVector ScComponentManagerCommandInstall::Execute(ScMemoryContext * context, ScAddr const & actionAddr)
{
  bool executionResult = true;
  std::vector<std::string> componentsToInstallIdentifiers;
  ScAddrVector componentsToInstall;
  ScAddrVector identifiersNodes =
      common_utils::CommonUtils::GetNodesUnderParameter(*context, actionAddr, scAgentsCommon::CoreKeynodes::rrel_1);

  if (identifiersNodes.empty())
  {
    SC_LOG_INFO("ScComponentManagerCommandInstall: No identifier provided, can't install");
    return identifiersNodes;
  }
  for (ScAddr const & identifierNode : identifiersNodes)
  {
    componentsToInstallIdentifiers.push_back(context->HelperGetSystemIdtf(identifierNode));
  }
  componentsToInstall = GetAvailableComponents(context, componentsToInstallIdentifiers);

  for (ScAddr componentAddr : componentsToInstall)
  {
    executionResult = InstallDependencies(context, componentAddr);
    executionResult &= DownloadComponent(context, componentAddr);
    if (executionResult)
    {
      executionResult &= InstallComponent(context, componentAddr);
    }
    // TODO: need to process installation method from component specification in kb
  }

  return identifiersNodes;
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

    ScAddr const actionAddr =
        utils::AgentUtils::formActionNode(context, keynodes::ScComponentManagerKeynodes::action_components_install, {});
    common_utils::CommonUtils::TransformToScStruct(*context, actionAddr, dependencyParameters);

    bool dependencyResult = Execute(context, actionAddr).empty();

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
  ScIterator3Ptr const & componentClassIterator =
      context->Iterator3(ScType::NodeConstClass, ScType::EdgeAccessConstPosPerm, componentAddr);
  while (componentClassIterator->Next())
  {
    componentClass = componentClassIterator->Get(0);
    if (m_componentsPath.count(componentClass) != 0)
    {
      m_downloadDir = m_componentsPath.at(componentClass);
      break;
    }
  }

  // Get url address from where to download component
  std::string urlAddress;
  ScAddr const & componentAddress = componentUtils::SearchUtils::GetComponentAddress(context, componentAddr);
  context->GetLinkContent(componentAddress, urlAddress);

  // Download component source code from GitHub repository
  std::shared_ptr<Downloader> m_downloader = std::make_shared<DownloaderGit>();
  bool result = m_downloader->DownloadRepository(m_downloadDir, urlAddress);

  // Load scs files from the new component directory
  std::stringstream downloadPath;
  RepositoryUrlParser parser;
  parser.Parse(urlAddress);
  std::string const repositoryName = parser.GetRepositoryName();
  std::string const directoryName = parser.GetDirectoryName();
  downloadPath << m_downloadDir << SpecificationConstants::DIRECTORY_DELIMITER << repositoryName
               << SpecificationConstants::DIRECTORY_DELIMITER << directoryName;
  if (!componentUtils::LoadUtils::LoadScsFilesInDir(
          context, downloadPath.str(), SpecificationConstants::SPECIFICATION_FILENAME))
  {
    SC_LOG_WARNING("ScComponentManagerCommandInstall: there is no *.scs files to load from " << downloadPath.str());
  }

  return result;
}
