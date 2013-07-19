#!/bin/bash

SELF_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
AOSP_DIR=$SELF_DIR/../../../../

echo "SELF_DIR=$SELF_DIR"
echo "AOSP_DIR=$AOSP_DIR"

# check prerequisite symlinks
if [ ! -L $SELF_DIR/third_party/android_tools/aosp ] ; then
    ln -f -s $AOSP_DIR $SELF_DIR/third_party/android_tools/aosp
fi

if [ ! -L $SELF_DIR/third_party/android_tools/sdk ] ; then
    echo "ERROR: fbstream incorrectly configured, missing SDK symlink. See the README."
    exit 1
fi

if [ ! -L $SELF_DIR/third_party/android_tools/ndk ] ; then
    echo "ERROR: fbstream incorrectly configured, missing NDK symlink. See the README."
    exit 1
fi

# Do the build

cd $SELF_DIR
#gclient sync --nohooks
export PATH=$PATH:../depot_tools
export WEBRTC_SKIP_RESOURCES_DOWNLOAD=1
. build/android/envsetup.sh --target-arch=x86
gclient runhooks

#ninja -C out/Debug -j 4  All
ninja -C out/Release -j 4  All
strip out/Release/fbstream_webrtc
