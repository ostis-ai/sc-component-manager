#!/usr/bin/env bash
set -eo pipefail

CURRENT_DIR=$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)
source "${CURRENT_DIR}/formats.sh"
source "${CURRENT_DIR}/set_vars.sh"

relative()
{
  realpath --relative-to="$(pwd)" "$1"
}

function usage() {
  cat <<USAGE
Usage: $(basename "$0") [OPTION]...

Options:
  -f, --force        full rebuild with the deleting of the $(relative "${BINARY_PATH}") and $(relative "${BUILD_PATH}") folders
  -r, --release      build in release mode
  --cmake-arg        add new argument into cmake build
  -h, --help         display this help and exit
USAGE
  exit 0
}

outer_cmake_args=()
while [ "$1" != "" ];
do
  case $1 in
    "-f"|"--force" )
      build_force=1
      ;;
    "-r"|"--release" )
      build_release=1
      ;;
    "--cmake-arg" )
      shift 1
      outer_cmake_args+=("${1}")
      ;;
    "-h"|"--help" )
      usage
      exit 0
      ;;
    * )
      echo -e "$(basename "$0"): unknown flag $1"
      echo "Try '$0 -h' for help"
      exit 2
      ;;
  esac
  shift 1
done

if ((build_force == 1));
then
  stage "Clear latest build"
  rm -rf "${SC_COMPONENT_MANAGER_BUILD_PATH}"
  find "${SC_COMPONENT_MANAGER_PATH}" -type d -name generated -exec rm -rf {} +
fi

release_mode="-DCMAKE_BUILD_TYPE=Release"

stage "Build sc-component-manager"

cd "${SC_COMPONENT_MANAGER_CMAKE_PATH}"
cmake -B "${SC_COMPONENT_MANAGER_BUILD_PATH}" "${SC_COMPONENT_MANAGER_CMAKE_PATH}" ${build_release:+${release_mode}} "${outer_cmake_args[@]}"
cmake --build "${SC_COMPONENT_MANAGER_BUILD_PATH}" -j"$(nproc)"

stage "Sc-component-manager built successfully"
