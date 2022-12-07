/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc_component_manager_command_install.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "src/manager/commands/command_init/constants/command_init_constants.hpp"

ExecutionResult ScComponentManagerCommandInstall::Execute(ScMemoryContext * context, CommandParameters const & commandParameters)
{
  ExecutionResult result;

  // TODO(MksmOrlov): if no params install all from config
  std::vector<std::string> componentsToInstall = commandParameters.at(PARAMETER_NAME);
  for (std::string const & componentToInstall : componentsToInstall)
  {
    ScAddr componentAddr = context->HelperFindBySystemIdtf(componentToInstall);
    if (!componentAddr.IsValid())
    {
      SC_LOG_WARNING("Component \"" + componentToInstall + "\" not found. Unable to install");
      continue;
    }

    ScAddr componentAddressAddr;
    std::string componentAddress;
    ScIterator5Ptr componentAddressIterator = context->Iterator5(
          componentAddr,
          ScType::EdgeDCommon,
          ScType::LinkConst,
          ScType::EdgeAccessConstPosPerm,
          keynodes::ScComponentManagerKeynodes::nrel_component_address);

    if (componentAddressIterator->Next())
    {
      componentAddressAddr = componentAddressIterator->Get(2);
      componentAddress = utils::CommonUtils::getLinkContent(context, componentAddressAddr);
    }
    else
    {
      SC_LOG_WARNING("Component \"" + componentToInstall + "\" address not found. Unable to install");
      continue;
    }

    if (componentAddress.find(GitHubConstants::GITHUB_PREFIX) != std::string::npos)
    {
      SC_LOG_WARNING("found github address");
    }

  }

  return result;
}
