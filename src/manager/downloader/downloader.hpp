/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

class Downloader
{
public:
  virtual bool Download(
      std::string const & downloadPath,
      std::string const & urlAddress,
      std::string const & pathPostfix = "") = 0;

  virtual ~Downloader() = default;
};
