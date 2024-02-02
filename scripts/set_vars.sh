#!/usr/bin/env bash

SC_COMPONENT_MANAGER_PATH=$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && cd .. && pwd)

if [[ -z "${BINARY_PATH}" || -z "${SC_MACHINE_PATH}" ]];
then
  SC_MACHINE_PATH="${SC_COMPONENT_MANAGER_PATH}/../sc-machine"
  source "${SC_MACHINE_PATH}/scripts/set_vars.sh"
fi

export ROOT_CMAKE_PATH="${SC_COMPONENT_MANAGER_PATH}"
export BINARY_PATH="${BINARY_PATH:-${ROOT_CMAKE_PATH}/bin}"
export BUILD_PATH="${ROOT_CMAKE_PATH}/build}"
