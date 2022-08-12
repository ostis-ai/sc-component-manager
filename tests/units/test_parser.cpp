/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "gtest/gtest.h"
#include "sc_component_manager_test.hpp"

TEST_F(ScComponentManagerParserTest, GetCommandType)
{
  std::string commandType;

  commandType = m_parser->Parse("components init").first;
  EXPECT_EQ(commandType, "init");

  commandType = m_parser->Parse("components init -p param").first;
  EXPECT_EQ(commandType, "init");

  commandType = m_parser->Parse("components init -p param -a author").first;
  EXPECT_EQ(commandType, "init");

  commandType = m_parser->Parse("components init --parameter param").first;
  EXPECT_EQ(commandType, "init");

  commandType = m_parser->Parse("components search").first;
  EXPECT_EQ(commandType, "search");

  EXPECT_ANY_THROW(m_parser->Parse("components"));
  EXPECT_ANY_THROW(m_parser->Parse("foo init"));
  EXPECT_ANY_THROW(m_parser->Parse("copmonents init"));
  EXPECT_ANY_THROW(m_parser->Parse("componentsinit"));

  std::pair<std::string, CommandParameters> parsed =
      m_parser->Parse("components init --class class1 class2 -a author1 -r");
  EXPECT_EQ(parsed.first, "init");
  EXPECT_EQ(parsed.second.size(), 3);
  EXPECT_EQ(parsed.second.at("class").size(), 2);
  EXPECT_EQ(parsed.second.at("a").size(), 1);
  EXPECT_EQ(parsed.second.at("r").size(), 0);
}
