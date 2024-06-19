/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <filesystem>

#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_type.hpp>
#include <sc-memory/sc_iterator.hpp>
#include <sc-builder/src/scs_loader.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/CommonUtils.hpp>

#include "sc_component_utils.hpp"

#include "module/keynodes/ScComponentManagerKeynodes.hpp"
#include "constants/command_constants.hpp"
#include "url_parser/repository_url_parser.hpp"

using namespace common_utils;

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
ScAddrUnorderedSet SearchUtils::GetComponentDependencies(ScMemoryContext * context, ScAddr const & componentAddr)
{
  ScAddrUnorderedSet componentDependencies;
  ScAddr componentDependenciesSet;

  ScIterator5Ptr const & componentDependenciesSetIterator = context->Iterator5(
      componentAddr,
      ScType::EdgeDCommonConst,
      ScType::NodeConst,
      ScType::EdgeAccessConstPosPerm,
      keynodes::ScComponentManagerKeynodes::nrel_component_dependencies);

  while (componentDependenciesSetIterator->Next())
  {
    componentDependenciesSet = componentDependenciesSetIterator->Get(2);
    ScIterator3Ptr const & componentDependenciesIterator =
        context->Iterator3(componentDependenciesSet, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    while (componentDependenciesIterator->Next())
    {
      componentDependencies.insert(componentDependenciesIterator->Get(2));
    }
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

/**
 * @brief Get vector of ScLinks with specification address.
 * @param context current sc-memory context
 * @param componentSpecificationAddr sc-addr of specification node
 * @return Vector of sc-addr for sc-links which contain url address,
 * return empty vector if no links found
 */
ScAddrVector SearchUtils::GetSpecificationAddress(ScMemoryContext * context, ScAddr const & componentSpecificationAddr)
{
  ScAddrVector specificationAddressLinks;

  ScIterator5Ptr const & alternativeAddressesSetIterator = context->Iterator5(
      componentSpecificationAddr,
      ScType::EdgeDCommonConst,
      ScType::NodeTuple,
      ScType::EdgeAccessConstPosPerm,
      keynodes::ScComponentManagerKeynodes::nrel_alternative_addresses);

  if (!alternativeAddressesSetIterator->Next())
  {
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "No alternative addresses set found");
  }

  ScAddr const & alternativeAddressesSet = alternativeAddressesSetIterator->Get(2);

  if (utils::CommonUtils::isEmpty(context, alternativeAddressesSet))
  {
    SC_THROW_EXCEPTION(utils::ExceptionAssert, "Alternative addresses set is empty");
  }

  ScAddr specificationAddressAddr =
      utils::IteratorUtils::getAnyByOutRelation(context, alternativeAddressesSet, scAgentsCommon::CoreKeynodes::rrel_1);

  if (!specificationAddressAddr.IsValid())
  {
    specificationAddressAddr = utils::IteratorUtils::getAnyFromSet(context, alternativeAddressesSet);
  }

  specificationAddressLinks =
      utils::IteratorUtils::getAllWithType(context, specificationAddressAddr, ScType::LinkConst);

  if (specificationAddressLinks.empty())
  {
    SC_THROW_EXCEPTION(utils::ExceptionAssert, "No sc-links connected with address node");
  }

  return specificationAddressLinks;
}

/**
 * @brief Get sc-addr of sc-link with repository address.
 * @param context current sc-memory context
 * @param componentSpecificationAddr sc-addr of repository node
 * @return Sc-addr of link that contains url address of repository.
 */
ScAddr SearchUtils::GetRepositoryAddress(ScMemoryContext * context, ScAddr const & repositoryAddr)
{
  ScAddr addressLinkAddr;

  ScIterator5Ptr const & repositoryAddressIterator = context->Iterator5(
      repositoryAddr,
      ScType::EdgeDCommonConst,
      ScType::NodeConst,
      ScType::EdgeAccessConstPosPerm,
      keynodes::ScComponentManagerKeynodes::nrel_repository_address);

  if (!repositoryAddressIterator->Next())
  {
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "No address found for repository");
  }

  ScAddr const & repositoryAddressAddr = repositoryAddressIterator->Get(2);

  ScIterator3Ptr const & addressLinkIterator =
      context->Iterator3(repositoryAddressAddr, ScType::EdgeAccessConstPosPerm, ScType::LinkConst);

  if (!addressLinkIterator->Next())
  {
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "No links for repository address found");
  }

  addressLinkAddr = addressLinkIterator->Get(2);

  return addressLinkAddr;
};

/**
 * Get sc-addr vector of components that need to be installed
 * @param context current sc-memory context
 * @return vector of component's sc-addrs
 */
ScAddrVector SearchUtils::GetNeedToInstallComponents(ScMemoryContext * context)
{
  return utils::IteratorUtils::getAllWithType(
      context, keynodes::ScComponentManagerKeynodes::concept_need_to_install_components, ScType::NodeConst);
}

/**
 * Check if component is reusable
 * @param context current sc-memory context
 * @param componentAddr component addr
 * @return true if component is reusable
 */
bool InstallUtils::IsReusable(ScMemoryContext * context, ScAddr const & componentAddr)
{
  bool result = true;
  ScIterator3Ptr const reusableComponentCLassIterator = context->Iterator3(
      keynodes::ScComponentManagerKeynodes::concept_reusable_component, ScType::EdgeAccessConstPosPerm, componentAddr);
  if (!reusableComponentCLassIterator->Next())
  {
    SC_LOG_WARNING("Component is not a reusable component.");
    result = false;
  }
  return result;
}

/**
 * Get installation scripts from component
 * @param context current sc-memory context
 * @param componentAddr component addr
 * @return vector of installation scripts
 */
std::vector<std::string> InstallUtils::GetInstallScripts(ScMemoryContext * context, ScAddr const & componentAddr)
{
  ScIterator5Ptr const & installScriptsIterator = context->Iterator5(
      componentAddr,
      ScType::EdgeDCommonConst,
      ScType::LinkConst,
      ScType::EdgeAccessConstPosPerm,
      keynodes::ScComponentManagerKeynodes::nrel_installation_script);

  // TODO: bug: in kb folder there are 2 specifications for one rep, so all links duplicated
  std::vector<std::string> scripts;
  while (installScriptsIterator->Next())
  {
    std::string script;
    ScAddr const & scriptAddrs = installScriptsIterator->Get(2);
    context->GetLinkContent(scriptAddrs, script);
    if (!script.empty())
    {
      scripts.push_back(script);
    }
    SC_LOG_DEBUG("ScComponentManager: Install script found:" + script);
  }
  return scripts;
}

/**
 * Check if component installation method valid
 * @param context current sc-memory context
 * @param componentAddr component addr
 * @return true if component installation method valid
 */
bool InstallUtils::IsComponentInstallationMethodValid(ScMemoryContext * context, ScAddr const & componentAddr)
{
  bool result = true;
  ScAddr const & componentInstallationMethod =
      componentUtils::SearchUtils::GetComponentInstallationMethod(context, componentAddr);
  if (!componentInstallationMethod.IsValid())
  {
    SC_LOG_WARNING("Component installation method isn't valid.");
    result = false;
  }
  return result;
}

/**
 * Get string of component address
 * @param context current sc-memory context
 * @param componentAddr component addr
 * @return string of component address
 */
std::string InstallUtils::GetComponentAddressStr(ScMemoryContext * context, ScAddr const & componentAddr)
{
  ScAddr const & componentAddressAddr = componentUtils::SearchUtils::GetComponentAddress(context, componentAddr);
  std::string componentAddressContent;
  context->GetLinkContent(componentAddressAddr, componentAddressContent);
  return componentAddressContent;
}

/**
 * Get component directory name
 * @param context current sc-memory context
 * @param componentAddr component addr
 * @param specificationsPath path to specification
 * @return component directory name
 */
std::string InstallUtils::GetComponentDirName(
    ScMemoryContext * context,
    ScAddr const & componentAddr,
    std::string const & specificationsPath)
{
  // Find a repositoryLink as https://github.com/MakarenkoAI/sc-web
  std::string componentAddressStr = GetComponentAddressStr(context, componentAddr);

  RepositoryUrlParser parser;
  parser.Parse(componentAddressStr);
  // Find a repositoryName as sc-web
  std::string const directoryName = parser.GetRepositoryName();

  std::stringstream componentsPath;
  componentsPath << specificationsPath << SpecificationConstants::DIRECTORY_DELIMITER << directoryName;

  return componentsPath.str();
}

/**
 * Load all .scs files in directory
 * @param context current sc-memory context
 * @param dirPath directory path
 */
bool LoadUtils::LoadScsFilesInDir(
    ScMemoryContext * context,
    std::string const & dirPath,
    std::string const & excludedFiles)
{
  bool result = false;
  ScsLoader loader;

  if (!std::filesystem::exists(dirPath))
  {
    return result;
  }

  for (std::filesystem::directory_entry const & dirEntry : std::filesystem::recursive_directory_iterator(dirPath))
  {
    std::filesystem::path const & filePath = dirEntry.path();
    if (filePath.extension() == SpecificationConstants::SCS_EXTENSION && filePath.filename().string() != excludedFiles)
    {
      // TODO: fix to "result = loader.loadScsFile(*context, filePath);" after merge in sc-machine
      loader.loadScsFile(*context, filePath);
      result = true;
    }
  }

  return result;
}

}  // namespace componentUtils
