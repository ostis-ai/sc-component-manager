/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <dirent.h>
#include <sys/stat.h>

#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_type.hpp>
#include <sc-memory/sc_iterator.hpp>
#include <sc-builder/src/scs_loader.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/CommonUtils.hpp>
#include "src/manager/commands/keynodes/ScComponentManagerKeynodes.hpp"
#include "sc_component_utils.hpp"

namespace componentUtils {

/**
 * @brief Get content of component address link content
 * @param context current sc-memory context
 * @param componentAddr sc-addr of the component to install
 * @return sc-addr of component address
 */
    ScAddr SearchUtils::GetComponentAddress(ScMemoryContext *context, ScAddr const &componentAddr)
    {
        ScAddr componentAddressAddr;
        ScIterator5Ptr const &componentAddressIterator = context->Iterator5(
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
    ScAddrVector SearchUtils::GetComponentDependencies(ScMemoryContext *context, ScAddr const &componentAddr)
    {
        ScAddrVector componentDependencies;
        ScAddrVector componentCurrentDependencies;
        ScAddr componentDependenciesSet;

        ScIterator5Ptr const &componentDependenciesIterator = context->Iterator5(
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
                    componentDependencies.cend(), componentCurrentDependencies.cbegin(),
                    componentCurrentDependencies.cend());
        }

        return componentDependencies;
    }

/**
 * @brief Get component installation method
 * @param context current sc-memory context
 * @param componentAddr sc-addr of the component to install
 * @return sc-addr of the component installation method
 */
    ScAddr SearchUtils::GetComponentInstallationMethod(ScMemoryContext *context, ScAddr const &componentAddr)
    {
        ScAddr componentInstallationMethod;
        ScIterator5Ptr const &componentDependenciesIterator = context->Iterator5(
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
    ScAddrVector
    SearchUtils::GetSpecificationAddress(ScMemoryContext *context, ScAddr const &componentSpecificationAddr)
    {
        ScAddrVector specificationAddressLinks;

        ScIterator5Ptr const &alternativeAddressesSetIterator = context->Iterator5(
                componentSpecificationAddr,
                ScType::EdgeDCommonConst,
                ScType::NodeTuple,
                ScType::EdgeAccessConstPosPerm,
                keynodes::ScComponentManagerKeynodes::nrel_alternative_addresses);

        if (!alternativeAddressesSetIterator->Next())
        {
            SC_LOG_ERROR("No alternative addresses set found");
            return specificationAddressLinks;
        }
        ScAddr const &alternativeAddressesSet = alternativeAddressesSetIterator->Get(2);

        if (utils::CommonUtils::isEmpty(context, alternativeAddressesSet))
        {
            SC_LOG_ERROR("Alternative addresses set is empty");
            return specificationAddressLinks;
        }

        ScAddr specificationAddressAddr = utils::IteratorUtils::getFirstFromSet(context, alternativeAddressesSet, true);

        if (!specificationAddressAddr.IsValid())
        {
            specificationAddressAddr = utils::IteratorUtils::getAnyFromSet(context, alternativeAddressesSet);
        }

        specificationAddressLinks =
                utils::IteratorUtils::getAllWithType(context, specificationAddressAddr, ScType::LinkConst);

        if (specificationAddressLinks.empty())
        {
            SC_LOG_ERROR("No sc-links connected with address node");
            return specificationAddressLinks;
        }

        return specificationAddressLinks;
    };

/**
 * @brief Get sc-addr of sc-link with repository address.
 * @param context current sc-memory context
 * @param componentSpecificationAddr sc-addr of repository node
 * @return Sc-addr of link that contains url address of repository.
 */
    ScAddr SearchUtils::GetRepositoryAddress(ScMemoryContext *context, ScAddr const &repositoryAddr)
    {
        ScAddr addressLinkAddr;

        ScIterator5Ptr const &repositoryAddressIterator = context->Iterator5(
                repositoryAddr,
                ScType::EdgeDCommonConst,
                ScType::NodeConst,
                ScType::EdgeAccessConstPosPerm,
                keynodes::ScComponentManagerKeynodes::nrel_repository_address);

        if (!repositoryAddressIterator->Next())
        {
            SC_LOG_ERROR("No address found for repository");
            return addressLinkAddr;
        }

        ScAddr const &repositoryAddressAddr = repositoryAddressIterator->Get(2);

        ScIterator3Ptr const &addressLinkIterator =
                context->Iterator3(repositoryAddressAddr, ScType::EdgeAccessConstPosPerm, ScType::LinkConst);

        if (!addressLinkIterator->Next())
        {
            SC_LOG_ERROR("No links for repository address found");
            return addressLinkAddr;
        }

        addressLinkAddr = addressLinkIterator->Get(2);

        return addressLinkAddr;
    };

    /**
     *
     * @param context
     * @param componentAddr
     * @return
     */
    bool InstallUtils::IsReusable(ScMemoryContext *context, ScAddr const &componentAddr)
    {
        bool result = true;
        ScIterator3Ptr const reusableComponentCLassIterator = context->Iterator3(
                keynodes::ScComponentManagerKeynodes::concept_reusable_component, ScType::EdgeAccessConstPosPerm,
                componentAddr);
        if (!reusableComponentCLassIterator->Next())
        {
            SC_LOG_WARNING("Component is not a reusable component.");
            result = false;
        }
        return result;
    }

    /**
     *
     * @param context
     * @param componentAddr
     * @return
     */
    std::vector <std::string> InstallUtils::GetInstallScripts(ScMemoryContext *context, ScAddr const &componentAddr)
    {
        ScIterator5Ptr const &installScriptsIterator = context->Iterator5(
                componentAddr,
                ScType::EdgeDCommonConst,
                ScType::LinkConst,
                ScType::EdgeAccessConstPosPerm,
                keynodes::ScComponentManagerKeynodes::nrel_install_scripts
        );

        //TODO: bug: in kb folder there are 2 specifications for one rep, so all links duplicated
        std::string script;
        while (installScriptsIterator->Next())
        {
            const ScAddr &scriptAddrs = installScriptsIterator->Get(2);
            context->GetLinkContent(scriptAddrs, script);
            SC_LOG_WARNING("ScComponentManager: Install script found:" + script);
        }
        return {};
    }

    /**
     *
     * @param context
     * @param componentAddr
     * @return
     */
    bool InstallUtils::IsComponentInstallationMethodValid(ScMemoryContext *context, ScAddr const &componentAddr)
    {
        bool result = true;
        ScAddr const &componentInstallationMethod =
                componentUtils::SearchUtils::GetComponentInstallationMethod(context, componentAddr);
        if (!componentInstallationMethod.IsValid())
        {
            SC_LOG_WARNING("Component installation method isn't valid.");
            result = false;
        }
        return result;
    }

    /**
     *
     * @param context
     * @param componentAddr
     * @return
     */
    std::string InstallUtils::GetComponentAddressStr(ScMemoryContext *context, ScAddr const &componentAddr)
    {
        ScAddr const &componentAddressAddr = componentUtils::SearchUtils::GetComponentAddress(context, componentAddr);
        std::string componentAddressContent;
        context->GetLinkContent(componentAddressAddr, componentAddressContent);
        return componentAddressContent;
    }

    /**
     *
     * @param context
     * @param componentAddr
     * @param specificationsPath
     * @return
     */
    std::string InstallUtils::GetComponentDirName(ScMemoryContext *context, ScAddr const &componentAddr,
                                                  const std::string &specificationsPath)
    {
        std::string const &componentAddressContent = componentUtils::InstallUtils::GetComponentAddressStr(context,
                                                                                                          componentAddr);
        size_t componentDirNameIndex = componentAddressContent.rfind('/');
        std::string componentDirName = specificationsPath + componentAddressContent.substr(componentDirNameIndex);
        return componentDirName;
    }

    /**
     *
     * @param context
     * @param dirPath
     */
    void LoadUtils::LoadScsFilesInDir(ScMemoryContext *context, std::string const &dirPath)
    {
        ScsLoader loader;
        DIR *dir;
        struct dirent *diread;
        if ((dir = opendir(dirPath.c_str())) != nullptr)
        {
            while ((diread = readdir(dir)) != nullptr)
            {
                std::string filename = diread->d_name;
                if (filename.rfind(".scs") != std::string::npos)
                {
                    loader.loadScsFile(*context, dirPath + "/" + filename);
                }
            }
            closedir(dir);
        }
    }

}  // namespace componentUtils
