# This file is generated by gyp; do not edit.

export builddir_name ?= trunk/webrtc/voice_engine/out
.PHONY: all
all:
	$(MAKE) -C ../.. voice_engine voe_cmd_test voe_auto_test voice_engine_unittests
