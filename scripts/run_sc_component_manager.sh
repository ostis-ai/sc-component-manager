#!/usr/bin/env bash

set -eo pipefail

CURRENT_DIR=$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)
source "${CURRENT_DIR}/formats.sh"
source "${CURRENT_DIR}/set_vars.sh"

INTERACTIVE_MODE=""

while getopts 'c:ih' OPTION;
do
  case "$OPTION" in
    c)
      CONFIG_PATH="$OPTARG"
      CONFIG_ABSOLUTE_PATH=$(cd "$(dirname "$CONFIG_PATH")" && pwd -P)/$(basename "$CONFIG_PATH")
      ;;
    i)
      INTERACTIVE_MODE="-i"
      ;;
    h)
      echo "script usage: $(basename \$0) [-c CONFIG_PATH] [-i]" >&2
      exit 1
      ;;
  esac
done
shift "$((OPTIND -1))"

if [ -z "$CONFIG_ABSOLUTE_PATH" ];
then
    CONFIG_ABSOLUTE_PATH="${CONFIG_PATH}"
fi

"${BINARY_PATH}/sc-component-manager" -c "${CONFIG_ABSOLUTE_PATH}" "${INTERACTIVE_MODE}"
