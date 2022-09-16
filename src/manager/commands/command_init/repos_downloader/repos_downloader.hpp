/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

class ReposDownloader
{
public:
  virtual void Download(
      std::string const & componentPath,
      std::string const & specificationsPath,
      bool is_repository) = 0;

  virtual ~ReposDownloader() = default;
};
