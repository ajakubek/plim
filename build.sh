#/bin/sh

INSTALL_PREFIX=${INSTALL_PREFIX:-"${PWD}/dist"}
BUILD_TYPE=${BUILD_TYPE:-debugfull}

mkdir "${INSTALL_PREFIX}" build &>/dev/null
cd build
cmake -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" .. && make && make install
