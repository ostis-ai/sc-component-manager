/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>

#include "sc_component_manager_test.hpp"
#include "../../src/manager/commands/url_parser/repository_url_parser.hpp"

TEST_F(ScComponentManagerTest, ParseRepositoryUrl)
{
  std::string const repositoryUrlAddress = "https://github.com/username/repository-name";
  RepositoryUrlParser parser;
  parser.Parse(repositoryUrlAddress);

  std::string const urlAddress = parser.GetUrlAddress();
  std::string const username = parser.GetUsername();
  std::string const repositoryName = parser.GetRepositoryName();
  std::string const directoryName = parser.GetDirectoryName();
  std::string const repositoryUrl = parser.GetRepositoryUrl();
  std::string const branchName = parser.GetBranchName();

  EXPECT_EQ(urlAddress, repositoryUrlAddress);
  EXPECT_EQ(username, "username");
  EXPECT_EQ(repositoryName, "repository-name");
  EXPECT_EQ(directoryName, "");
  EXPECT_EQ(repositoryUrl, repositoryUrlAddress);
  EXPECT_EQ(branchName, "");
}

TEST_F(ScComponentManagerTest, ParseRepositoryRealUrl)
{
  std::string const repositoryUrlAddress = "https://github.com/MksmOrlov/ostis-kb-lib";
  RepositoryUrlParser parser;
  parser.Parse(repositoryUrlAddress);

  std::string const urlAddress = parser.GetUrlAddress();
  std::string const username = parser.GetUsername();
  std::string const repositoryName = parser.GetRepositoryName();
  std::string const directoryName = parser.GetDirectoryName();
  std::string const repositoryUrl = parser.GetRepositoryUrl();
  std::string const branchName = parser.GetBranchName();

  EXPECT_EQ(urlAddress, repositoryUrlAddress);
  EXPECT_EQ(username, "MksmOrlov");
  EXPECT_EQ(repositoryName, "ostis-kb-lib");
  EXPECT_EQ(directoryName, "");
  EXPECT_EQ(repositoryUrl, repositoryUrlAddress);
  EXPECT_EQ(branchName, "");
}

TEST_F(ScComponentManagerTest, ParseRepositoryUrlWithDirectory)
{
  std::string const repositoryUrlAddress = "https://github.com/MksmOrlov/ostis-kb-lib/tree/main/part_methods_tools";
  RepositoryUrlParser parser;
  parser.Parse(repositoryUrlAddress);

  std::string const urlAddress = parser.GetUrlAddress();
  std::string const username = parser.GetUsername();
  std::string const repositoryName = parser.GetRepositoryName();
  std::string const directoryName = parser.GetDirectoryName();
  std::string const repositoryUrl = parser.GetRepositoryUrl();
  std::string const branchName = parser.GetBranchName();

  EXPECT_EQ(urlAddress, repositoryUrlAddress);
  EXPECT_EQ(username, "MksmOrlov");
  EXPECT_EQ(repositoryName, "ostis-kb-lib");
  EXPECT_EQ(directoryName, "part_methods_tools");
  EXPECT_EQ(branchName, "main");
  EXPECT_EQ(repositoryUrl, "https://github.com/MksmOrlov/ostis-kb-lib");
}

TEST_F(ScComponentManagerTest, ParseRepositoryUrlWithSubdirectory)
{
  std::string const repositoryUrlAddress =
      "https://github.com/username/repository-name/tree/main/directory/subdirectory";
  RepositoryUrlParser parser;
  parser.Parse(repositoryUrlAddress);

  std::string const urlAddress = parser.GetUrlAddress();
  std::string const username = parser.GetUsername();
  std::string const repositoryName = parser.GetRepositoryName();
  std::string const directoryName = parser.GetDirectoryName();
  std::string const repositoryUrl = parser.GetRepositoryUrl();
  std::string const branchName = parser.GetBranchName();

  EXPECT_EQ(urlAddress, repositoryUrlAddress);
  EXPECT_EQ(username, "username");
  EXPECT_EQ(repositoryName, "repository-name");
  EXPECT_EQ(directoryName, "directory/subdirectory");
  EXPECT_EQ(branchName, "main");
  EXPECT_EQ(repositoryUrl, "https://github.com/username/repository-name");
}
