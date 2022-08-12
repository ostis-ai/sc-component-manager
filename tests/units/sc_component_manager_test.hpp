/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "gtest/gtest.h"

#include "sc-memory/sc_memory.hpp"

#include "src/manager/parser/sc_component_manager_parser.hpp"

#include <memory>

class ScComponentManagerParserTest : public testing::Test
{
protected:
  void SetUp() override
  {
  }

  void TearDown() override
  {
  }

protected:
  std::unique_ptr<ScComponentManagerParser> m_parser;
};
