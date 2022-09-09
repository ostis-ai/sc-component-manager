/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "gtest/gtest.h"

#include "sc_component_manager_test.hpp"

#include "test_defines.hpp"
#include "src/manager/commands/command_init/repos_downloader/repos_downloader_handler.hpp"

TEST_F(ScComponentManagerParserTest, ParseReposFile)
{
  m_reposParser->Parse(REPOS_PATH "/test_repos.txt");

  std::vector<std::string> repositories = {"https://github.com/MksmOrlov/components-repo-example/repos.txt"};
  std::vector<std::string> components = {
      "https://github.com/MksmOrlov/cat-kb-component",
      "https://drive.google.com/file/d/1eUxPBd3VKNFSDEgolz11EQdEmBJMFYIk/view?usp=sharing"};

  std::vector<std::string> parsedRepositories = m_reposParser->GetRepositories();
  std::vector<std::string> parsedComponents = m_reposParser->GetComponents();

  EXPECT_TRUE(parsedRepositories.size() == 1);
  EXPECT_TRUE(parsedComponents.size() == 2);
  EXPECT_EQ(parsedRepositories, repositories);
  EXPECT_EQ(parsedComponents, components);
  //
  //  ReposDownloaderHandler handler;
  //  handler.Handle("https://drive.google.com/file/d/1eUxPBd3VKNFSDEgolz11EQdEmBJMFYIk/view?usp=sharing");
}
