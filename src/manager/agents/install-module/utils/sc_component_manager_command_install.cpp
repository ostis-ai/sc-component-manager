/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager_command_install.hpp"

#include <sc-memory/utils/sc_exec.hpp>

#include "utils/sc_component_utils.hpp"

#include "module/utils/common_utils.hpp"

#include "sc-agents-common/utils/AgentUtils.hpp"
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

using namespace common_utils;

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
ScAddrUnorderedSet ScComponentManagerCommandInstall::GetAvailableComponents(
    ScMemoryContext * context,
    ScAddrUnorderedSet const & componentsToInstall)
{
  ScAddrUnorderedSet availableComponents;
  for (ScAddr const & componentToInstall : componentsToInstall)
  {
    try
    {
      SC_LOG_DEBUG(
          "ScComponentManagerCommandInstall: Validating component \""
          << context->HelperGetSystemIdtf(componentToInstall) << "\"");
      ValidateComponent(context, componentToInstall);
    }
    catch (utils::ScException const & exception)
    {
      SC_LOG_ERROR("ScComponentManagerCommandInstall: Unable to install component");
      SC_LOG_DEBUG(exception.Message());
      continue;
    }
    SC_LOG_DEBUG(
        "ScComponentManagerCommandInstall: Component \"" << context->HelperGetSystemIdtf(componentToInstall)
                                                         << "\" is specified correctly");
    availableComponents.insert(componentToInstall);
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

ScAddrUnorderedSet ScComponentManagerCommandInstall::Execute(ScMemoryContext * context, ScAddr const & actionAddr)
{
  bool executionResult = true;
  ScAddr const & parameterNode =
      utils::IteratorUtils::getAnyByOutRelation(context, actionAddr, scAgentsCommon::CoreKeynodes::rrel_1);
  ScAddrUnorderedSet componentsToInstall = CommonUtils::GetComponentsToInstall(*context, parameterNode);

  if (componentsToInstall.empty())
  {
    SC_LOG_INFO("ScComponentManagerCommandInstall: No identifier provided, can't install");
    return componentsToInstall;
  }

  componentsToInstall = GetAvailableComponents(context, componentsToInstall);

  ScAddr decompositionAddr;
  for (ScAddr const & componentAddr : componentsToInstall)
  {
    if (CommonUtils::CheckIfInstalled(*context, componentAddr))
    {
      SC_LOG_DEBUG("Component \"" << context->HelperGetSystemIdtf(componentAddr) << "\" is already installed");
      continue;
    }
    executionResult = InstallDependencies(context, componentAddr);
    executionResult &= DownloadComponent(context, componentAddr);
    if (executionResult)
    {
      executionResult &= InstallComponent(context, componentAddr);
    }
    // TODO: need to process installation method from component specification in kb
    decompositionAddr = CommonUtils::GetSubsystemDecompositionAddr(*context, componentAddr);
    if (context->IsElement(decompositionAddr))
      context->CreateEdge(ScType::EdgeAccessConstPosPerm, decompositionAddr, componentAddr);
    else
      SC_LOG_WARNING(
          "Component \"" << context->HelperGetSystemIdtf(componentAddr) << "\" can't be added in myself-decomposition");
  }

  return componentsToInstall;
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

ScAddr ScComponentManagerCommandInstall::CreateInstallStructureAndGetDepsSet(
    ScMemoryContext * context,
    ScAddr const & actionAddr)
{
  ScAddr const & mainParameter = context->CreateNode(ScType::NodeConst);
  ScAddr const & setsParameter = context->CreateNode(ScType::NodeConst);
  ScAddr const & depSetsParameter = context->CreateNode(ScType::NodeConst);
  ScAddr edge;

  edge = context->CreateEdge(ScType::EdgeAccessConstPosPerm, actionAddr, mainParameter);
  context->CreateEdge(ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::rrel_1, edge);

  edge = context->CreateEdge(ScType::EdgeAccessConstPosPerm, mainParameter, setsParameter);
  context->CreateEdge(ScType::EdgeAccessConstPosPerm, keynodes::ScComponentManagerKeynodes::rrel_sets, edge);

  context->CreateEdge(ScType::EdgeAccessConstPosPerm, setsParameter, depSetsParameter);

  return depSetsParameter;
}

bool ScComponentManagerCommandInstall::InstallDependencies(ScMemoryContext * context, ScAddr const & componentAddr)
{
  bool result;
  // Get component dependencies and install them recursively
  ScAddrUnorderedSet const & componentDependencies =
      componentUtils::SearchUtils::GetComponentDependencies(context, componentAddr);

  ScAddr const & actionAddr =
      utils::AgentUtils::formActionNode(context, keynodes::ScComponentManagerKeynodes::action_components_install, {});
  ScAddr const & depsSetParameter = CreateInstallStructureAndGetDepsSet(context, actionAddr);

  for (ScAddr const & componentDependency : componentDependencies)
  {
    std::string dependencyIdtf = context->HelperGetSystemIdtf(componentDependency);
    context->CreateEdge(ScType::EdgeAccessConstPosPerm, depsSetParameter, componentDependency);
    SC_LOG_INFO("ScComponentManager: Found dependency \"" << dependencyIdtf << "\"");
  }

  // Call install components agent
  utils::AgentUtils::applyAction(context, actionAddr, 30000);

  result = context->HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_successfully, actionAddr, ScType::EdgeAccessConstPosPerm);
  if (!result)
  {
    SC_LOG_ERROR("ScComponentManagerCommandInstall: Dependencies are not successfully installed");
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
