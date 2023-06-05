#!/usr/bin/env bash

set -eo pipefail

pip3 install --user -r requirements.txt

cmake -B build -DSC_FILE_MEMORY=${FILE_MEMORY} -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DSC_COVERAGE=${COVERAGE} -DSC_AUTO_TEST=ON -DSC_BUILD_TESTS=ON
echo ::group::Make
cmake --build build -j$(nproc)
echo ::endgroup::

pushd build
../bin/sc-component-manager -i ../sc-tools/sc-component-manager/tests -o ../bin/sc-component-manager-test-repo --clear -f
popd
