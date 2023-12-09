/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>

#include "sc_component_manager_test.hpp"
#include "src/manager/commands/sc_component_manager_command.hpp"

TEST_F(ScComponentManagerTest, ParseCommandTypes)
{
  std::string commandType;

  commandType = m_commandParser->Parse("components init").first;
  EXPECT_EQ(commandType, "init");

  commandType = m_commandParser->Parse("components init -p param").first;
  EXPECT_EQ(commandType, "init");

  commandType = m_commandParser->Parse("components init --parameter param").first;
  EXPECT_EQ(commandType, "init");

  commandType = m_commandParser->Parse("components init -f").first;
  EXPECT_EQ(commandType, "init");

  commandType = m_commandParser->Parse("components search").first;
  EXPECT_EQ(commandType, "search");

  commandType = m_commandParser->Parse("components install").first;
  EXPECT_EQ(commandType, "install");
}

TEST_F(ScComponentManagerTest, ParseCommandParameters)
{
  std::pair<std::string, CommandParameters> parsed =
      m_commandParser->Parse("components search --class concept_kb_component");
  EXPECT_EQ(parsed.second.size(), (size_t)1);
  EXPECT_EQ(parsed.second.at("class").size(), (size_t)1);
  EXPECT_TRUE(parsed.second.at("class").at(0) == "concept_kb_component");

  parsed = m_commandParser->Parse("components search --class class1 class2 -a author1 -r");
  EXPECT_EQ(parsed.second.size(), (size_t)3);
  EXPECT_EQ(parsed.second.at("class").size(), (size_t)2);
  EXPECT_EQ(parsed.second.at("a").size(), (size_t)1);
  EXPECT_EQ(parsed.second.at("r").size(), (size_t)0);
}

TEST_F(ScComponentManagerTest, ParseIncorrectCommands)
{
  EXPECT_ANY_THROW(m_commandParser->Parse("components"));
  EXPECT_ANY_THROW(m_commandParser->Parse("foo init"));
  EXPECT_ANY_THROW(m_commandParser->Parse("copmonents init"));
  EXPECT_ANY_THROW(m_commandParser->Parse("componentsinit"));
  EXPECT_ANY_THROW(m_commandParser->Parse("search components"));
}
