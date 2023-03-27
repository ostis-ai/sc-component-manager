/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager_command_install.hpp"
#include <sc-memory/utils/sc_exec.hpp>
#include <sc-builder/src/scs_loader.hpp>
#include "src/manager/utils/sc_component_utils.hpp"

#include "src/manager/commands/command_init/constants/command_init_constants.hpp"


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
ScAddrVector ScComponentManagerCommandInstall::GetAvailableComponents(ScMemoryContext *context,
                                                                      std::vector <std::string> componentsToInstall)
{
    ScAddrVector availableComponents;
    for (std::string const &componentToInstallIdentifier: componentsToInstall)
    {
        ScAddr componentAddr = context->HelperFindBySystemIdtf(componentToInstallIdentifier);

        SC_LOG_DEBUG("Validating component \"" + componentToInstallIdentifier);
        if (!ValidateComponent(context, componentAddr))
        {
            SC_LOG_WARNING("Unable to install component \"" + componentToInstallIdentifier);
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
 * @param componentAddr component addr
 */
void ScComponentManagerCommandInstall::InstallComponent(ScMemoryContext *context, ScAddr const &componentAddr)
{
    std::vector <std::string> scripts = componentUtils::InstallUtils::GetInstallScripts(context, componentAddr);
    for (auto script: scripts)
    {
        std::string componentDirName = componentUtils::InstallUtils::GetComponentDirName(context, componentAddr,
                                                                                         m_specificationsPath);
        sc_fs_mkdirs(componentDirName.c_str());
        ScExec exec{{"cd", componentDirName, "&&", script}};
    }
}


ExecutionResult ScComponentManagerCommandInstall::Execute(
        ScMemoryContext *context,
        CommandParameters const &commandParameters)
{
    ExecutionResult executionResult; //TODO: result doesn't change
    std::vector <std::string> componentsToInstall;

    try
    {
        componentsToInstall = commandParameters.at(PARAMETER_NAME);
    }
    catch (std::exception const &exception)
    {
        // TODO: Implement install all components method
        SC_LOG_INFO("No identifier provided, installing all to install components");

        return executionResult;
    }

    ScAddrVector availableComponents = GetAvailableComponents(context, componentsToInstall);

    for (ScAddr componentAddr: availableComponents)
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
 * @return Returns true if component is valid.
 */
bool ScComponentManagerCommandInstall::ValidateComponent(ScMemoryContext *context, ScAddr const &componentAddr)
{
    bool result = true;
    // Check if component exist
    if (!componentAddr.IsValid())
    {
        SC_LOG_WARNING("Component not found. Unable to install");
        result = false;
    }

    // Check if component is a reusable component
    if (result && !componentUtils::InstallUtils::IsReusable(context, componentAddr))
    {
        SC_LOG_WARNING("Component is not a reusable component.");
        result = false;
    }

    // Find and check component address
    if (result && componentUtils::InstallUtils::GetComponentAddressStr(context, componentAddr).empty())
    {
        SC_LOG_WARNING("Component address not found.");
        result = false;
    }

    // Find and check component installation method
    if (result && !componentUtils::InstallUtils::IsComponentInstallationMethodValid(context, componentAddr))
    {
        SC_LOG_WARNING("Component installation method not found.");
        result = false;
    }

    return result;
}

/**
 * Tries to install component dependencies.
 * @return Returns {DependencyIdtf1, DependencyIdtf2, ...}
 * if installation successfull, otherwise
 * returns empty vector.
 */
ExecutionResult ScComponentManagerCommandInstall::InstallDependencies(
        ScMemoryContext *context,
        ScAddr const &componentAddr)
{
    ExecutionResult result;
    // Get component dependencies and install them recursively
    ScAddrVector const &componentDependencies =
            componentUtils::SearchUtils::GetComponentDependencies(context, componentAddr);
    for (ScAddr const &componentDependency: componentDependencies)
    {
        std::string dependencyIdtf = context->HelperGetSystemIdtf(componentDependency);
        SC_LOG_INFO("ScComponentManager: Install dependency \"" + dependencyIdtf + "\"");
        CommandParameters dependencyParameters = {{PARAMETER_NAME, {dependencyIdtf}}};
        ExecutionResult dependencyResult = Execute(context, dependencyParameters);

        // Return empty if you couldn't install dependency why?
        if (dependencyResult.empty())
        {
            SC_LOG_ERROR("Dependency \"" + dependencyIdtf + "\" is not installed");
            //return dependencyResult;
        } else
        {
            result.insert(result.cbegin(), dependencyResult.cbegin(), dependencyResult.cend());
        }
    }

    return result;
}

/**
 * Tries to download component from Github
 */
void ScComponentManagerCommandInstall::DownloadComponent(ScMemoryContext *context, ScAddr const &componentAddr)
{
    std::string const &componentAddressContent = componentUtils::InstallUtils::GetComponentAddressStr(context,
                                                                                                      componentAddr);
    if (componentAddressContent.find(GitHubConstants::GITHUB_PREFIX) != std::string::npos)
    {
        sc_fs_mkdirs(m_specificationsPath.c_str());
        ScExec exec{{"cd", m_specificationsPath, "&&", "git clone ", componentAddressContent}};

        std::string componentDirName = componentUtils::InstallUtils::GetComponentDirName(context, componentAddr,
                                                                                         m_specificationsPath);
        componentUtils::LoadUtils::LoadScsFilesInDir(context, componentDirName);
        //TODO: need to check if all files are translated and write warning if not, bug in sc-machine loader
    }
}
