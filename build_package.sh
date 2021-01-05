#!/bin/bash

###################################################
# generate a package installer for AnimationMaker #
###################################################

if [[ $# -ne 2 ]]
then
    echo "usage: ${0} (rpm | deb) <path to build directory>"

    exit 1
fi

BUILD_DIR="${2}"
TYPE="${1}"
SOURCE_DIR="${BUILD_DIR}/output"
PLUGIN_DIR="${SOURCE_DIR}/plugins"
INSTALL_PREFIX="/opt/animationmaker/"
PACKAGE_DESCRIPTION="AnimationMaker is a software designed to help you to quickly build animations video.\n\n
These animations can be exported to a video file/gif image or you can build your own export plugin."

if [[ ! -d "${SOURCE_DIR}" ]]
then
    echo "Build dir '${BUILD_DIR}' doesn't exists or is not accessible"

    exit 1
fi

if [[ $TYPE != "deb" && $TYPE != "rpm" ]]
then
    echo "usage: ${0} (rpm | deb) <path to build directory>"

    exit 1
fi

if [[ ! -d "${PLUGIN_DIR}" ]]
then
    echo "Plugin folder not found, build package without plugins"
fi

# rename the binary to lowercase letter to comply with deb packages
mv ${SOURCE_DIR}/AnimationMaker ${SOURCE_DIR}/animationmaker

if [[ "${TYPE}" == "deb" ]]
then
    deps=(
        "-d" "libqt5core5a"
        "-d" "libqt5gui5"
        "-d" "libqt5xml5"
        "-d" "libqt5widgets5"
        "-d" "libqt5svg5"
    )
else
    deps=(
        "-d" "qt5-qtbase"
        "-d" "qt5-qtbase-gui"
        "-d" "qt5-qtsvg"
    )
fi

fpm \
    -t "${TYPE}" \
    -s dir \
    -C "${SOURCE_DIR}" \
    --prefix "${INSTALL_PREFIX}"  \
    -n "animationmaker" \
    -m "Alexandre LAVIGNE" \
    --description "${PACKAGE_DESCRIPTION}" \
    --url "https://github.com/Artanidos/AnimationMaker" \
    --version 1.9 \
    ${deps[@]} \
    animationmaker plugins
