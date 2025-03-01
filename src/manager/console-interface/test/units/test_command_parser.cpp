/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>

#include "sc_component_manager_test.hpp"

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

  commandType = m_commandParser->Parse("components search -").first;
  EXPECT_EQ(commandType, "search");

  commandType = m_commandParser->Parse("components install").first;
  EXPECT_EQ(commandType, "install");
}

TEST_F(ScComponentManagerTest, ParseCommandParameters)
{
  std::pair<std::string, CommandParameters> parsed =
      m_commandParser->Parse("components search --class concept_kb_component");
  EXPECT_EQ(parsed.second.size(), 1u);
  EXPECT_EQ(parsed.second.at("class").size(), 1u);
  EXPECT_TRUE(parsed.second.at("class").at(0) == "concept_kb_component");

  parsed = m_commandParser->Parse("components search --class class1 class2 -a author1 -r");
  EXPECT_EQ(parsed.second.size(), 3u);
  EXPECT_EQ(parsed.second.at("class").size(), 2u);
  EXPECT_EQ(parsed.second.at("a").size(), 1u);
  EXPECT_EQ(parsed.second.at("r").size(), 0u);
}

TEST_F(ScComponentManagerTest, ParseIncorrectCommands)
{
  EXPECT_ANY_THROW(m_commandParser->Parse("components"));
  EXPECT_ANY_THROW(m_commandParser->Parse("foo init"));
  EXPECT_ANY_THROW(m_commandParser->Parse("copmonents init"));
  EXPECT_ANY_THROW(m_commandParser->Parse("componentsinit"));
  EXPECT_ANY_THROW(m_commandParser->Parse("search components"));
}

TEST_F(ScComponentManagerTest, ParseInstallCommandsFlag)
{
  std::string commandType;
  std::map<std::string, std::vector<std::string>> commandValues;
  std::pair<std::string, std::map<std::string, std::vector<std::string>>> commandWithValues;

  commandWithValues = m_commandParser->Parse("components install   sc_web ");
  commandType = commandWithValues.first;
  commandValues = commandWithValues.second;
  EXPECT_EQ(commandType, "install");
  EXPECT_EQ(commandValues["idtf"][0], "sc_web");
  EXPECT_EQ(commandValues.size(), 1u);

  commandWithValues = m_commandParser->Parse("components install --idtf sc_web");
  commandType = commandWithValues.first;
  commandValues = commandWithValues.second;
  EXPECT_EQ(commandType, "install");
  EXPECT_EQ(commandValues["idtf"][0], "sc_web");
  EXPECT_EQ(commandValues.size(), 1u);

  commandWithValues = m_commandParser->Parse("cinst  --idtf  sc_web");
  commandType = commandWithValues.first;
  commandValues = commandWithValues.second;
  EXPECT_EQ(commandType, "install");
  EXPECT_EQ(commandValues["idtf"][0], "sc_web");
  EXPECT_EQ(1u, commandValues.size());

  commandWithValues = m_commandParser->Parse("cinst  sc_web");
  commandType = commandWithValues.first;
  commandValues = commandWithValues.second;
  EXPECT_EQ(commandType, "install");
  EXPECT_EQ(commandValues["idtf"][0], "sc_web");
  EXPECT_EQ(1u, commandValues.size());

  commandWithValues = m_commandParser->Parse("comp inst --idtf sc_web");
  commandType = commandWithValues.first;
  commandValues = commandWithValues.second;
  EXPECT_EQ(commandType, "install");
  EXPECT_EQ(commandValues["idtf"][0], "sc_web");
  EXPECT_EQ(1u, commandValues.size());

  commandWithValues = m_commandParser->Parse("comp inst  sc_web");
  commandType = commandWithValues.first;
  commandValues = commandWithValues.second;
  EXPECT_EQ(commandType, "install");
  EXPECT_EQ(commandValues["idtf"][0], "sc_web");
  EXPECT_EQ(1u, commandValues.size());

  commandWithValues = m_commandParser->Parse(" components install");
  commandType = commandWithValues.first;
  commandValues = commandWithValues.second;
  EXPECT_EQ(commandType, "install");
  EXPECT_EQ(commandValues.size(), 0u);
}

TEST_F(ScComponentManagerTest, ParseSearchCommandExplanationFlag)
{
  std::string commandType;
  std::map<std::string, std::vector<std::string>> commandValues;
  std::pair<std::string, std::map<std::string, std::vector<std::string>>> commandWithValues;

  commandWithValues = m_commandParser->Parse("components search --explanation SC");
  commandType = commandWithValues.first;
  commandValues = commandWithValues.second;
  EXPECT_EQ(commandType, "search");
  EXPECT_EQ(commandValues["explanation"][0], "SC");
  EXPECT_EQ(commandValues.size(), 1u);

  commandWithValues = m_commandParser->Parse("components search --explanation \"SC\"");
  commandType = commandWithValues.first;
  commandValues = commandWithValues.second;
  EXPECT_EQ(commandType, "search");
  EXPECT_EQ(commandValues["explanation"][0], "SC");
  EXPECT_EQ(commandValues.size(), 1u);

  EXPECT_NO_THROW(m_commandParser->Parse("components search --explanation"));
  commandWithValues = m_commandParser->Parse("components search --explanation");
  commandType = commandWithValues.first;
  commandValues = commandWithValues.second;
  EXPECT_EQ(commandType, "search");
  EXPECT_EQ(commandValues["explanation"].size(), 0u);
}
