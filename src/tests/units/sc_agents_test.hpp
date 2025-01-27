/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/test/sc_test.hpp>

#include <memory>
#include <algorithm>
#include <unordered_set>
#include <filesystem>

#include <sc-builder/scs_loader.hpp>
#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_agent_context.hpp>

class ScAgentsTest : public testing::Test
{
public:
  static inline std::string const & TEST_FILES_DIR_PATH = "../test-structures/";
  static inline std::string const & TEST_FILES_COMPONENTS_SEARCH_DIR_PATH = "../test-structures/components-search/";
  static inline std::string const & TEST_KB_BIN_PATH = "../kb.bin/";
  static inline int const WAIT_TIME = 5000;
  static inline ScsLoader loader;

protected:
  virtual void SetUp()
  {
    ScAgentsTest::Initialize();
    m_ctx = std::make_unique<ScAgentContext>();
  }

  virtual void TearDown()
  {
    if (m_ctx)
      m_ctx->Destroy();

    ScAgentsTest::Shutdown();

    std::filesystem::remove_all(TEST_KB_BIN_PATH);
  }

  static void Initialize()
  {
    sc_memory_params params;
    sc_memory_params_clear(&params);

    params.dump_memory = SC_FALSE;
    params.dump_memory_statistics = SC_FALSE;

    params.clear = SC_TRUE;
    params.storage = TEST_KB_BIN_PATH.c_str();

    ScMemory::LogMute();
    ScMemory::Initialize(params);
    ScMemory::LogUnmute();
  }

  static void Shutdown()
  {
    ScMemory::LogMute();
    ScMemory::Shutdown(SC_FALSE);
    ScMemory::LogUnmute();
  }

protected:
  std::unique_ptr<ScAgentContext> m_ctx;
};
