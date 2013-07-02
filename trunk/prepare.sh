#gclient sync --nohooks
# TODO this following line will produce make files compatible with Android.
#export GYP_GENERATORS=android
export GYP_GENERATORS=ninja
#export ANDROID_NDK_ROOT=`pwd`/ndk
#export ANDROID_SDK_ROOT=`pwd`/sdk
export PATH=$PATH:../depot_tools
export WEBRTC_SKIP_RESOURCES_DOWNLOAD=1
build/android/envsetup.sh --target-arch=x86
gclient runhooks

ninja  -C out/Debug -j 4  All
#ninja -C out/Release -j 4  All
