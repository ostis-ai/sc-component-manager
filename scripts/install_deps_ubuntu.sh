#!/usr/bin/env bash

set -eo pipefail

packages=(
  libglib2.0-dev
  libboost-system-dev
  libboost-filesystem-dev
  libboost-program-options-dev
  make
  cmake
  ccache
  llvm
  gcc
  g++
  clang
  libclang-dev
  clang-format
)

sudo apt-get update && sudo apt-get install -y --no-install-recommends software-properties-common
sudo add-apt-repository -y universe
sudo apt-get update

sudo apt-get install -y --no-install-recommends "${packages[@]}"
sudo apt autoremove
