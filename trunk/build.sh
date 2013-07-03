#gclient sync --nohooks
export PATH=$PATH:../depot_tools
export WEBRTC_SKIP_RESOURCES_DOWNLOAD=1
. build/android/envsetup.sh --target-arch=x86
gclient runhooks

ninja -C out/Debug -j 4  All
#ninja -C out/Release -j 4  All
