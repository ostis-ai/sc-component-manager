#!/usr/bin/env bash
set -eo pipefail

CURRENT_DIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)"
OSTIS_SCRIPTS_DIR="${CURRENT_DIR}/ostis-scripts"
source "${OSTIS_SCRIPTS_DIR}/message-scripts/messages.sh"

if [[ -z "${SC_COMPONENT_MANAGER_CMAKE_PATH}" || -z "${SC_COMPONENT_MANAGER_CXX_SOURCES_PATH}"
  || -z "${SC_COMPONENT_MANAGER_BUILD_PATH}" || -z "${BINARY_PATH}" ]];
then
  source "${CURRENT_DIR}/set_vars.sh"
fi

info "Build sc-component-manager"

BUILD_SCRIPTS="${OSTIS_SCRIPTS_DIR}/build-scripts"
"${BUILD_SCRIPTS}/build_cxx_project.sh" -p "${SC_COMPONENT_MANAGER_CMAKE_PATH}" -s "${SC_COMPONENT_MANAGER_CXX_SOURCES_PATH}" \
  -b "${SC_COMPONENT_MANAGER_BUILD_PATH}" --cmake-arg "-DSC_BIN_PATH=${BINARY_PATH}" "$@"

info "sc-component-manager is built successfully"
