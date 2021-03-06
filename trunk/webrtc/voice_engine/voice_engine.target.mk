# This file is generated by gyp; do not edit.

TOOLSET := target
TARGET := voice_engine
DEFS_Debug := \
	'-DANGLE_DX11' \
	'-DWEBRTC_SVNREVISION="Unavailable(issue687)"' \
	'-D_FILE_OFFSET_BITS=64' \
	'-DUSE_LINUX_BREAKPAD' \
	'-DCHROMIUM_BUILD' \
	'-DUSE_DEFAULT_RENDER_THEME=1' \
	'-DUSE_LIBJPEG_TURBO=1' \
	'-DUSE_NSS=1' \
	'-DUSE_X11=1' \
	'-DENABLE_ONE_CLICK_SIGNIN' \
	'-DGTK_DISABLE_SINGLE_INCLUDES=1' \
	'-DENABLE_REMOTING=1' \
	'-DENABLE_WEBRTC=1' \
	'-DENABLE_PEPPER_CDMS' \
	'-DENABLE_CONFIGURATION_POLICY' \
	'-DENABLE_INPUT_SPEECH' \
	'-DENABLE_NOTIFICATIONS' \
	'-DENABLE_GPU=1' \
	'-DENABLE_EGLIMAGE=1' \
	'-DENABLE_TASK_MANAGER=1' \
	'-DENABLE_EXTENSIONS=1' \
	'-DENABLE_PLUGIN_INSTALLATION=1' \
	'-DENABLE_PLUGINS=1' \
	'-DENABLE_SESSION_SERVICE=1' \
	'-DENABLE_THEMES=1' \
	'-DENABLE_BACKGROUND=1' \
	'-DENABLE_AUTOMATION=1' \
	'-DENABLE_GOOGLE_NOW=1' \
	'-DENABLE_LANGUAGE_DETECTION=1' \
	'-DENABLE_PRINTING=1' \
	'-DENABLE_CAPTIVE_PORTAL_DETECTION=1' \
	'-DENABLE_MANAGED_USERS=1' \
	'-DWEBRTC_LOGGING' \
	'-DWEBRTC_LINUX' \
	'-D__STDC_CONSTANT_MACROS' \
	'-D__STDC_FORMAT_MACROS' \
	'-DDYNAMIC_ANNOTATIONS_ENABLED=1' \
	'-DWTF_USE_DYNAMIC_ANNOTATIONS=1' \
	'-D_DEBUG'

# Flags passed to all source files.
CFLAGS_Debug := \
	-fstack-protector \
	--param=ssp-buffer-size=4 \
	-Werror \
	-pthread \
	-fno-exceptions \
	-fno-strict-aliasing \
	-Wall \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-fvisibility=hidden \
	-pipe \
	-fPIC \
	-Wextra \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-O0 \
	-g

# Flags passed to only C files.
CFLAGS_C_Debug :=

# Flags passed to only C++ files.
CFLAGS_CC_Debug := \
	-fno-rtti \
	-fno-threadsafe-statics \
	-fvisibility-inlines-hidden \
	-Wsign-compare \
	-Woverloaded-virtual

INCS_Debug := \
	-Iwebrtc \
	-I. \
	-I. \
	-Iwebrtc/voice_engine/include \
	-Iwebrtc/modules/audio_device \
	-Iwebrtc/common_audio/resampler/include \
	-Iwebrtc/common_audio/signal_processing/include \
	-Iwebrtc/common_audio/vad/include \
	-Iwebrtc/modules/audio_coding/main/interface \
	-Iwebrtc/modules/interface \
	-Iwebrtc/modules/audio_conference_mixer/interface \
	-Iwebrtc/modules/audio_device/include \
	-Iwebrtc/modules/audio_processing/include \
	-Iwebrtc/modules/media_file/interface \
	-Iwebrtc/modules/rtp_rtcp/interface \
	-Iwebrtc/modules/utility/interface \
	-Iwebrtc/system_wrappers/interface

DEFS_Release := \
	'-DANGLE_DX11' \
	'-DWEBRTC_SVNREVISION="Unavailable(issue687)"' \
	'-D_FILE_OFFSET_BITS=64' \
	'-DUSE_LINUX_BREAKPAD' \
	'-DCHROMIUM_BUILD' \
	'-DUSE_DEFAULT_RENDER_THEME=1' \
	'-DUSE_LIBJPEG_TURBO=1' \
	'-DUSE_NSS=1' \
	'-DUSE_X11=1' \
	'-DENABLE_ONE_CLICK_SIGNIN' \
	'-DGTK_DISABLE_SINGLE_INCLUDES=1' \
	'-DENABLE_REMOTING=1' \
	'-DENABLE_WEBRTC=1' \
	'-DENABLE_PEPPER_CDMS' \
	'-DENABLE_CONFIGURATION_POLICY' \
	'-DENABLE_INPUT_SPEECH' \
	'-DENABLE_NOTIFICATIONS' \
	'-DENABLE_GPU=1' \
	'-DENABLE_EGLIMAGE=1' \
	'-DENABLE_TASK_MANAGER=1' \
	'-DENABLE_EXTENSIONS=1' \
	'-DENABLE_PLUGIN_INSTALLATION=1' \
	'-DENABLE_PLUGINS=1' \
	'-DENABLE_SESSION_SERVICE=1' \
	'-DENABLE_THEMES=1' \
	'-DENABLE_BACKGROUND=1' \
	'-DENABLE_AUTOMATION=1' \
	'-DENABLE_GOOGLE_NOW=1' \
	'-DENABLE_LANGUAGE_DETECTION=1' \
	'-DENABLE_PRINTING=1' \
	'-DENABLE_CAPTIVE_PORTAL_DETECTION=1' \
	'-DENABLE_MANAGED_USERS=1' \
	'-DWEBRTC_LOGGING' \
	'-DWEBRTC_LINUX' \
	'-D__STDC_CONSTANT_MACROS' \
	'-D__STDC_FORMAT_MACROS' \
	'-DNDEBUG' \
	'-DNVALGRIND' \
	'-DDYNAMIC_ANNOTATIONS_ENABLED=0' \
	'-D_FORTIFY_SOURCE=2'

# Flags passed to all source files.
CFLAGS_Release := \
	-fstack-protector \
	--param=ssp-buffer-size=4 \
	-Werror \
	-pthread \
	-fno-exceptions \
	-fno-strict-aliasing \
	-Wall \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-fvisibility=hidden \
	-pipe \
	-fPIC \
	-Wextra \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-O2 \
	-fno-ident \
	-fdata-sections \
	-ffunction-sections

# Flags passed to only C files.
CFLAGS_C_Release :=

# Flags passed to only C++ files.
CFLAGS_CC_Release := \
	-fno-rtti \
	-fno-threadsafe-statics \
	-fvisibility-inlines-hidden \
	-Wsign-compare \
	-Woverloaded-virtual

INCS_Release := \
	-Iwebrtc \
	-I. \
	-I. \
	-Iwebrtc/voice_engine/include \
	-Iwebrtc/modules/audio_device \
	-Iwebrtc/common_audio/resampler/include \
	-Iwebrtc/common_audio/signal_processing/include \
	-Iwebrtc/common_audio/vad/include \
	-Iwebrtc/modules/audio_coding/main/interface \
	-Iwebrtc/modules/interface \
	-Iwebrtc/modules/audio_conference_mixer/interface \
	-Iwebrtc/modules/audio_device/include \
	-Iwebrtc/modules/audio_processing/include \
	-Iwebrtc/modules/media_file/interface \
	-Iwebrtc/modules/rtp_rtcp/interface \
	-Iwebrtc/modules/utility/interface \
	-Iwebrtc/system_wrappers/interface

OBJS := \
	$(obj).target/$(TARGET)/webrtc/voice_engine/channel.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/channel_manager.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/channel_manager_base.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/dtmf_inband.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/dtmf_inband_queue.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/level_indicator.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/monitor_module.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/output_mixer.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/output_mixer_internal.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/shared_data.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/statistics.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/transmit_mixer.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/utility.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/voe_audio_processing_impl.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/voe_base_impl.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/voe_call_report_impl.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/voe_codec_impl.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/voe_dtmf_impl.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/voe_encryption_impl.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/voe_external_media_impl.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/voe_file_impl.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/voe_hardware_impl.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/voe_neteq_stats_impl.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/voe_network_impl.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/voe_rtp_rtcp_impl.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/voe_video_sync_impl.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/voe_volume_control_impl.o \
	$(obj).target/$(TARGET)/webrtc/voice_engine/voice_engine_impl.o

# Add to the list of files we specially track dependencies for.
all_deps += $(OBJS)

# CFLAGS et al overrides must be target-local.
# See "Target-specific Variable Values" in the GNU Make manual.
$(OBJS): TOOLSET := $(TOOLSET)
$(OBJS): GYP_CFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_C_$(BUILDTYPE))
$(OBJS): GYP_CXXFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_CC_$(BUILDTYPE))

# Suffix rules, putting all outputs into $(obj).

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(srcdir)/%.cc FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# Try building from generated source, too.

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj).$(TOOLSET)/%.cc FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj)/%.cc FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# End of this set of suffix rules
### Rules for final target.
LDFLAGS_Debug := \
	-Wl,-z,now \
	-Wl,-z,relro \
	-pthread \
	-Wl,-z,noexecstack \
	-fPIC \
	-Wl,--threads \
	-Wl,--thread-count=4 \
	-B$(builddir)/../../third_party/gold \
	-Wl,--icf=none

LDFLAGS_Release := \
	-Wl,-z,now \
	-Wl,-z,relro \
	-pthread \
	-Wl,-z,noexecstack \
	-fPIC \
	-Wl,--threads \
	-Wl,--thread-count=4 \
	-B$(builddir)/../../third_party/gold \
	-Wl,--icf=none \
	-Wl,-O1 \
	-Wl,--as-needed \
	-Wl,--gc-sections

LIBS := \
	

$(obj).target/webrtc/voice_engine/libvoice_engine.a: GYP_LDFLAGS := $(LDFLAGS_$(BUILDTYPE))
$(obj).target/webrtc/voice_engine/libvoice_engine.a: LIBS := $(LIBS)
$(obj).target/webrtc/voice_engine/libvoice_engine.a: TOOLSET := $(TOOLSET)
$(obj).target/webrtc/voice_engine/libvoice_engine.a: $(OBJS) FORCE_DO_CMD
	$(call do_cmd,alink_thin)

all_deps += $(obj).target/webrtc/voice_engine/libvoice_engine.a
# Add target alias
.PHONY: voice_engine
voice_engine: $(obj).target/webrtc/voice_engine/libvoice_engine.a

# Add target alias to "all" target.
.PHONY: all
all: voice_engine

