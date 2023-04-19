/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_memory.hpp>

namespace componentUtils
{

class SearchUtils
{
public:
  static ScAddr GetComponentAddress(ScMemoryContext * context, ScAddr const & componentAddr);

  static ScAddrVector GetComponentDependencies(ScMemoryContext * context, ScAddr const & componentAddr);

  static ScAddr GetComponentInstallationMethod(ScMemoryContext * context, ScAddr const & componentAddr);

  static ScAddrVector GetSpecificationAddress(ScMemoryContext * context, ScAddr const & componentSpecificationAddr);

  static ScAddr GetRepositoryAddress(ScMemoryContext * context, ScAddr const & repositoryAddr);

  static ScAddrVector GetNeedToInstallComponents(ScMemoryContext * context);
};

class InstallUtils
{
public:
  static bool IsReusable(ScMemoryContext * context, ScAddr const & componentAddr);

  static bool IsComponentInstallationMethodValid(ScMemoryContext * context, ScAddr const & componentAddr);

  static std::string GetComponentAddressStr(ScMemoryContext * context, ScAddr const & componentAddr);

  static std::string GetComponentDirName(
      ScMemoryContext * context,
      ScAddr const & componentAddr,
      const std::string & specificationsPath);

  static std::vector<std::string> GetInstallScripts(ScMemoryContext * context, ScAddr const & componentAddr);
};

class LoadUtils
{
public:
  static bool LoadScsFilesInDir(ScMemoryContext * context, std::string const & dirPath);
};

}  // namespace componentUtils
