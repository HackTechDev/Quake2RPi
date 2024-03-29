# ------------------------------------------------------ #
# Makefile for the "Yamagi Quake 2 Client"               #
#                                                        #
# Just type "make" to compile the                        #
#  - SDL Client (quake2)                                 #
#  - Server (q2ded)                                      #
#  - SDL OpenGL-Refresher (ref_gl.so)                    #
#  - Quake II Game (baseq2)                              #
#                                                        #
# Base dependencies:                                     #
#  - SDL 1.2                                             #
#  - libGL                                               #
#                                                        #
# Platforms:                                             #
#  - Linux                                               #
#  - FreeBSD                                             #
# ------------------------------------------------------ #

# User configurable options
# -------------------------

# The default options should be okay for most users!
# Do not change anything, unless you know what you're
# doing! Do not complain if using non-default settings
# break your game!

# Enables CD audio playback. CD audio playback is used
# for the background music and doesn't add any further
# dependencies. It should work on all platforms where
# CD playback is supported by SDL.
WITH_CDA := no

# OpenGL ES instead of "normal" OpenGL. This implies
# WITH_STATICQGL and WITH_VERTEXARRAYS.
WITH_GLES := yes

# Enables OGG/Vorbis support. OGG/Vorbis files can be
# used as a substitute of CD audio playback. Adds
# dependencies to libogg, libvorbis and libvorbisfile.
WITH_OGG := no

# Enables the optional OpenAL sound system.
# To use it your system needs libopenal.so.1 (we 
# recommend openal-soft) installed
WITH_OPENAL := no

# Enables retexturing support. Adds a dependency to
# libjpeg
WITH_RETEXTURING := yes

# When this option is set, libGL is not loaded at 
# runtime. This is meant for embedded platforms and
# shall not be used on "normal" desktop systems!
WITH_STATICQGL := YES

# Enable the vertex array code path in the renderer
# instead of the classic one. This may not work on
# very old (older than about 1999) graphic cards!
WITH_VERTEXARRAYS := yes

# Set the gamma via X11 and not via SDL. This works
# around problems in some SDL version. Adds dependencies
# to pkg-config, libX11 and libXxf86vm
WITH_X11GAMMA := no

# Enables opening of ZIP files (also known as .pk3 packs).
# Adds a dependency to libz
WITH_ZIP := yes

# Enable systemwide installation of game assets
WITH_SYSTEMWIDE := no

# This will set the default SYSTEMDIR, a non-empty 
# string would actually be used
WITH_SYSTEMDIR := ""

# ------------------------------------------------------ #

# Those a predefined settings for several handheld and 
# embedded platforms. Enabling one of these (and ONLY 
# one, more will break the build process!) will select
# the correct settings (see above) automatically! 
# DESKTOP USERS DO NOT WANT TO ENABLE ANY OF THESE!

# The OpenPandora handheld console
PANDORA := no

# The GamePark CAANOO
CAANOO := no

# The GamePark Wiz
WIZ := no

# Raspberry PI
PI := yes

# if you are using Raspbian or another distribution with hardware 
# floating point support, set that to yes
HARDFLOAT := yes

# ------------------------------------------------------ #

# Those variables can be filled to configure the build
# environment. Those are only usefull of the game is
# cross compiled. All pathes need a terminating slash 
# (/)!

# The compiler to use
#CC =

# Path to your dev-kit
#PREFIX :=

# Path to the toolchain (relativ to PREFIX)
#TOOLS :=

# ====================================================== #
#     !!! DO NOT ALTER ANYTHING BELOW THIS LINE !!!      #
# ====================================================== #

# Check the OS type
OSTYPE := $(shell uname -s)

# Some platforms call it "amd64" and some "x86_64"
ARCH := $(shell uname -m | sed -e s/i.86/i386/ -e s/amd64/x86_64/)

# Refuse all other platforms as a firewall against PEBKAC
# (You'll need some #ifdef for your unsupported  plattform!)
ifeq ($(findstring $(ARCH), i386 x86_64 sparc64 armv6l),)
$(error arch $(ARCH) is currently not supported)
endif

# ----------

# Base CFLAGS.
#
# -O2 are enough optimizations.
#
# -fno-strict-aliasing since the source doesn't comply
#  with strict aliasing rules and it's next to impossible
#  to get it there...
#
# -fomit-frame-pointer since the framepointer is mostly
#  useless for debugging Quake II and slows things down.
#
# -g to build always with debug symbols. Please DO NOT
#  CHANGE THIS, since it's our only chance to debug this
#  crap when random crashes happen!
#
# -MMD to generate header dependencies.
CFLAGS := -O2 -fno-strict-aliasing -fomit-frame-pointer \
		  -Wall -pipe -g -MMD

# ----------

# Systemwide installation
ifeq ($(WITH_SYSTEMWIDE),yes)
CFLAGS += -DSYSTEMWIDE
ifneq ($(WITH_SYSTEMDIR),"")
CFLAGS += -DSYSTEMDIR=\"$(WITH_SYSTEMDIR)\"
endif
endif

# ----------

# Predefined build profiles
ifeq ($(PANDORA),yes)
WITH_GLES = yes
WITH_STATICQGL = yes
CFLAGS += -DPANDORA -DUSE_EGL_SDL
endif

ifeq ($(CAANOO),yes)
WITH_GLES = yes
WITH_STATICQGL = yes
CFLAGS += -DCAANOO -DUSE_EGL_RAW
endif

ifeq ($(WIZ),yes)
WITH_GLES = yes
WITH_STATICQGL = yes
CFLAGS += -DWIZ -DUSE_EGL_RAW
endif

ifeq ($(PI),yes)
WITH_GLES = yes
WITH_STATICQGL = yes
CFLAGS += -mtune=arm1176jzf-s -mfpu=vfp  -DUSE_EGL_RAW -DUSE_VCHIQ_ARM -DUSE_GLES2 -DPI -DEGL_VERSION_1_2


endif

ifeq ($(HARDFLOAT), yes)
CFLAGS += -mfloat-abi=hard
else
CFLAGS += -mfloat-abi=softfp
endif

# ----------

# Extra CFLAGS for SDL
ifneq ($(PANDORA),yes)
ifneq ($(CAANOO),yes)
ifneq ($(WIZ),yes)
SDLCFLAGS := $(shell $(PREFIX)$(TOOLS)sdl-config --cflags)
endif
endif
endif

# ----------

# Extra CFLAGS for X11
ifeq ($(WITH_X11GAMMA),yes)
X11CFLAGS := $(shell $(PREFIX)/$(TOOLS)/pkg-config x11 --cflags)
X11CFLAGS += $(shell $(PREFIX)/$(TOOLS)/pkg-config xxf86vm --cflags)
else
X11CFLAGS :=
endif

# ----------

# Base include path.
ifeq ($(OSTYPE),Linux)
ifeq ($(PANDORA),yes)
INCLUDE := -I/usr/include -I$(PREFIX)/usr/include -I$(PREFIX)/usr/include/SDL
else ifeq ($(CAANOO),yes)
INCLUDE := -I/usr/include -I$(PREFIX)/DGE/include -I$(PREFIX)/DGE/include/SDL
else ifeq ($(WIZ),yes)
INCLUDE := -I/usr/include -I$(PREFIX)/include -I$(PREFIX)/include/SDL
else ifeq ($(PI),yes)
INCLUDE := -I$(PREFIX)/opt/vc/include/interface/vcos/pthreads -I$(PREFIX)/opt/vc/include -I/usr/include -I$(PREFIX)/usr/include/SDL -I/opt/vc/include/interface/vmcs_host/linux
else # Normal Linux
INCLUDE := -I/usr/include
endif
else ifeq ($(OSTYPE),FreeBSD)
INCLUDE := -I/usr/local/include
endif

# ----------

# Base LDFLAGS.
ifeq ($(OSTYPE),Linux)
ifeq ($(PANDORA),yes)
LDFLAGS := -L$(PREFIX)/usr/lib -lSDL -lts -lm -ldl
else ifeq ($(CAANOO),yes)
LDFLAGS := -L$(PREFIX)/DGE/lib/target -lSDL -lm -ldl
else ifeq ($(WIZ),yes)
LDFLAGS := -L$(PREFIX)/lib -lSDL -lm -ldl
else ifeq ($(PI),yes)
LDFLAGS := -L/opt/vc/lib -L/usr/lib -lm -ldl -lGLESv2 -lEGL -lvcos -lvchiq_arm -lSDL -lrt -lbcm_host -lkhrn_static -lopenmaxil -lvchostif
else # Normal Linux
LDFLAGS := -L/usr/lib -lm -ldl
endif
else ifeq ($(OSTYPE),FreeBSD)
LDFLAGS := -L/usr/local/lib -lm
endif

# ----------

# Extra LDFLAGS for SDL
ifneq ($(PANDORA),yes)
ifneq ($(CAANOO),yes)
ifneq ($(WIZ),yes)
SDLLDFLAGS := $(shell $(PREFIX)$(TOOLS)sdl-config --libs)
endif
endif
endif

# ----------

# Extra LDFLAGS for X11
ifeq ($(WITH_X11GAMMA),yes)
X11LDFLAGS := $(shell pkg-config x11 --libs)
X11LDFLAGS += $(shell pkg-config xxf86vm --libs)
else
X11LDFLAGS :=
endif

# ----------

# When make is invoked by "make VERBOSE=1" print
# the compiler and linker commands.

ifdef VERBOSE
Q :=
else
Q := @
endif

# ----------

# Builds everything
all: client server refresher game

# ----------

# Cleanup
clean:
	@echo "===> CLEAN"
	${Q}rm -Rf build release

# ----------

# The client
client:
	@echo '===> Building quake2'
	${Q}mkdir -p release
	$(MAKE) release/quake2

build/client/%.o: %.c
	@echo '===> CC $<'
	${Q}mkdir -p $(@D)
	${Q}$(CC) -c $(CFLAGS) $(SDLCFLAGS) $(INCLUDE) -o $@ $<

ifeq ($(WITH_CDA),yes)
release/quake2 : CFLAGS += -DCDA
endif

ifeq ($(WITH_OGG),yes)
release/quake2 : CFLAGS += -DOGG
release/quake2 : LDFLAGS += -lvorbis -lvorbisfile -logg
endif

ifeq ($(WITH_OPENAL),yes)
release/quake2 : CFLAGS += -DUSE_OPENAL -DDEFAULT_OPENAL_DRIVER='"libopenal.so.1"'
endif

ifeq ($(WITH_ZIP),yes)
release/quake2 : CFLAGS += -DZIP
release/quake2 : LDFLAGS += -lz
endif

# ----------

# The server
server:
	@echo '===> Building q2ded'
	${Q}mkdir -p release
	$(MAKE) release/q2ded

build/server/%.o: %.c
	@echo '===> CC $<'
	${Q}mkdir -p $(@D)
	${Q}$(CC) -c $(CFLAGS) $(INCLUDE) -o $@ $<

release/q2ded : CFLAGS += -DDEDICATED_ONLY
release/q2ded : LDFLAGS += -lz

ifeq ($(WITH_ZIP),yes)
release/q2ded : CFLAGS += -DZIP
release/q2ded : LDFLAGS += -lz
endif

# ----------

# The refresher
refresher:
	@echo '===> Building ref_gl.so'
	${Q}mkdir -p release
	$(MAKE) release/ref_gl.so

build/refresher/%.o: %.c
	@echo '===> CC $<'
	${Q}mkdir -p $(@D)
	${Q}$(CC) -c $(CFLAGS) $(SDLCFLAGS) $(X11CFLAGS) $(INCLUDE) -o $@ $<

release/ref_gl.so : CFLAGS += -fPIC
release/ref_gl.so : LDFLAGS += -shared
 
ifeq ($(WITH_GLES),yes)
release/ref_gl.so : CFLAGS += -DQGL_DIRECT_LINK -DGLES -DGLES_ONLY -DVERTEX_ARRAYS
endif	
 
ifeq ($(WITH_RETEXTURING),yes)
release/ref_gl.so : CFLAGS += -DRETEXTURE
release/ref_gl.so : LDFLAGS += -ljpeg
endif
  
ifeq ($(WITH_STATICQGL),yes)
release/ref_gl.so : CFLAGS += -DQGL_DIRECT_LINK
endif
 
ifeq ($(WITH_VERTEXARRAYS),yes)
release/ref_gl.so : CFLAGS += -DVERTEX_ARRAYS
endif
 
ifeq ($(WITH_X11GAMMA),yes)
release/ref_gl.so : CFLAGS += -DX11GAMMA
endif 

# ----------

# The baseq2 game
game:
	@echo '===> Building baseq2/game.so'
	${Q}mkdir -p release/baseq2
	$(MAKE) release/baseq2/game.so

build/baseq2/%.o: %.c
	@echo '===> CC $<'
	${Q}mkdir -p $(@D)
	${Q}$(CC) -c $(CFLAGS) $(INCLUDE) -o $@ $<

release/baseq2/game.so : CFLAGS += -fPIC
release/baseq2/game.so : LDFLAGS += -shared

# ----------

# Used by the game
GAME_OBJS_ = \
	src/common/shared/flash.o \
	src/common/shared/shared.o \
    src/game/g_ai.o \
	src/game/g_chase.o \
	src/game/g_cmds.o \
	src/game/g_combat.o \
	src/game/g_func.o \
	src/game/g_items.o \
	src/game/g_main.o \
	src/game/g_misc.o \
	src/game/g_monster.o \
	src/game/g_phys.o \
	src/game/g_spawn.o \
	src/game/g_svcmds.o \
	src/game/g_target.o \
	src/game/g_trigger.o \
	src/game/g_turret.o \
	src/game/g_utils.o \
	src/game/g_weapon.o \
	src/game/monster/berserker/berserker.o \
	src/game/monster/boss2/boss2.o \
	src/game/monster/boss3/boss3.o \
	src/game/monster/boss3/boss31.o \
	src/game/monster/boss3/boss32.o \
	src/game/monster/brain/brain.o \
	src/game/monster/chick/chick.o \
	src/game/monster/flipper/flipper.o \
	src/game/monster/float/float.o \
	src/game/monster/flyer/flyer.o \
	src/game/monster/gladiator/gladiator.o \
	src/game/monster/gunner/gunner.o \
	src/game/monster/hover/hover.o \
	src/game/monster/infantry/infantry.o \
	src/game/monster/insane/insane.o \
	src/game/monster/medic/medic.o \
	src/game/monster/misc/move.o \
	src/game/monster/mutant/mutant.o \
	src/game/monster/parasite/parasite.o \
	src/game/monster/soldier/soldier.o \
	src/game/monster/supertank/supertank.o \
	src/game/monster/tank/tank.o \
	src/game/player/client.o \
	src/game/player/hud.o \
	src/game/player/trail.o \
	src/game/player/view.o \
	src/game/player/weapon.o \
    src/game/savegame/savegame.o

# ----------

# Used by the client
CLIENT_OBJS_ := \
	src/client/cl_cin.o \
	src/client/cl_console.o \
	src/client/cl_download.o \
	src/client/cl_effects.o \
	src/client/cl_entities.o \
	src/client/cl_input.o \
	src/client/cl_inventory.o \
	src/client/cl_keyboard.o \
	src/client/cl_lights.o \
	src/client/cl_main.o \
	src/client/cl_network.o \
	src/client/cl_parse.o \
	src/client/cl_particles.o \
	src/client/cl_prediction.o \
	src/client/cl_screen.o \
	src/client/cl_tempentities.o \
	src/client/cl_view.o \
	src/client/menu/menu.o \
	src/client/menu/qmenu.o \
	src/client/menu/videomenu.o \
	src/client/sound/snd_al.o \
	src/client/sound/snd_dma.o \
	src/client/sound/snd_mem.o \
	src/client/sound/snd_mix.o \
	src/client/sound/snd_vorbis.o \
	src/client/sound/snd_wav.o \
	src/common/crc.o \
	src/common/cvar.o \
	src/common/filesystem.o \
	src/common/md4.o \
	src/common/misc.o \
	src/common/netchan.o \
	src/common/pmove.o \
	src/common/szone.o \
	src/common/zone.o \
	src/common/command/cmd_execution.o \
	src/common/command/cmd_parser.o \
	src/common/command/cmd_script.o \
	src/common/common/com_arg.o \
	src/common/common/com_clientserver.o \
	src/common/message/msg_io.o \
	src/common/message/msg_read.o \
 	src/common/model/cm_areaportals.o \
	src/common/model/cm_box.o \
	src/common/model/cm_boxtracing.o \
	src/common/model/cm_bsp.o \
	src/common/model/cm_vis.o \
	src/common/shared/flash.o \
	src/common/shared/shared.o \
	src/common/unzip/ioapi.o \
	src/common/unzip/unzip.o \
	src/sdl/cd.o \
	src/sdl/sound.o \
	src/server/sv_cmd.o \
	src/server/sv_conless.o \
	src/server/sv_entities.o \
	src/server/sv_game.o \
	src/server/sv_init.o \
	src/server/sv_main.o \
	src/server/sv_save.o \
	src/server/sv_send.o \
	src/server/sv_user.o \
	src/server/sv_world.o \
	src/unix/glob.o \
	src/unix/hunk.o \
	src/unix/main.o \
 	src/unix/network.o \
	src/unix/qal.o \
 	src/unix/signalhandler.o \
	src/unix/system.o \
 	src/unix/vid.o

# ----------

# Used by the server
SERVER_OBJS_ := \
	src/common/crc.o \
	src/common/cvar.o \
	src/common/filesystem.o \
	src/common/md4.o \
	src/common/misc.o \
	src/common/netchan.o \
	src/common/pmove.o \
	src/common/szone.o \
	src/common/zone.o \
	src/common/command/cmd_execution.o \
	src/common/command/cmd_parser.o \
	src/common/command/cmd_script.o \
	src/common/common/com_arg.o \
	src/common/common/com_clientserver.o \
	src/common/message/msg_io.o \
	src/common/message/msg_read.o \
 	src/common/model/cm_areaportals.o \
	src/common/model/cm_box.o \
	src/common/model/cm_boxtracing.o \
	src/common/model/cm_bsp.o \
	src/common/model/cm_vis.o \
	src/common/shared/shared.o \
	src/common/unzip/ioapi.o \
	src/common/unzip/unzip.o \
	src/server/sv_cmd.o \
	src/server/sv_conless.o \
	src/server/sv_entities.o \
	src/server/sv_game.o \
	src/server/sv_init.o \
	src/server/sv_main.o \
	src/server/sv_save.o \
	src/server/sv_send.o \
	src/server/sv_user.o \
	src/server/sv_world.o \
	src/unix/glob.o \
	src/unix/hunk.o \
	src/unix/main.o \
 	src/unix/network.o \
 	src/unix/signalhandler.o \
	src/unix/system.o

# ----------

# Used by the OpenGL refresher
OPENGL_OBJS_ = \
	src/refresh/r_draw.o \
	src/refresh/r_image.o \
	src/refresh/r_light.o \
	src/refresh/r_lightmap.o \
	src/refresh/r_main.o \
	src/refresh/r_mesh.o \
	src/refresh/r_misc.o \
	src/refresh/r_model.o \
	src/refresh/r_scrap.o \
	src/refresh/r_surf.o \
	src/refresh/r_warp.o \
	src/refresh/files/md2.o \
	src/refresh/files/pcx.o \
	src/refresh/files/sp2.o \
	src/refresh/files/tga.o \
	src/refresh/files/jpeg.o \
	src/refresh/files/wal.o \
	src/sdl/input.o \
	src/sdl/refresh.o \
    src/common/shared/shared.o \
    src/unix/glob.o \
	src/unix/hunk.o

ifeq ($(WITH_GLES),yes)
ifeq ($(PANDOR),yes)
OPENGL_OBJS_ += \
	src/egl/eglport.o
else ifeq ($(CAANOO),yes)
OPENGL_OBJS_ += \
	src/egl/eglport.o src/egl/in_gph.o
else ifeq ($(WIZ),yes)
OPENGL_OBJS_ += \
	src/egl/eglport.o src/egl/in_gph.o
else ifeq ($(PI),yes)
OPENGL_OBJS_ += \
	src/egl/eglport.o
else # Normal OpenGL ES
OPENGL_OBJS_ += \
	src/unix/qgles.o
endif
else
OPENGL_OBJS_ += \
	src/unix/qgl.o
endif

# ----------

# Rewrite pathes to our object directory
CLIENT_OBJS = $(patsubst %,build/client/%,$(CLIENT_OBJS_))
SERVER_OBJS = $(patsubst %,build/server/%,$(SERVER_OBJS_))
OPENGL_OBJS = $(patsubst %,build/refresher/%,$(OPENGL_OBJS_))
GAME_OBJS = $(patsubst %,build/baseq2/%,$(GAME_OBJS_))

# ----------

# Generate header dependencies
CLIENT_DEPS= $(CLIENT_OBJS:.o=.d)
SERVER_DEPS= $(SERVER_OBJS:.o=.d)
OPENGL_DEPS= $(OPENGL_OBJS:.o=.d)
GAME_DEPS= $(GAME_OBJS:.o=.d)

# ----------

# Suck header dependencies in
-include $(CLIENT_DEPS)
-include $(SERVER_DEPS)
-include $(OPENGL_DEPS)
-include $(GAME_DEPS)

# ----------

# release/quake2
release/quake2 : $(CLIENT_OBJS)
	@echo '===> LD $@'
	${Q}$(CC) $(CLIENT_OBJS) $(LDFLAGS) $(SDLLDFLAGS) -o $@

# release/q2ded
release/q2ded : $(SERVER_OBJS)
	@echo '===> LD $@'
	${Q}$(CC) $(SERVER_OBJS) $(LDFLAGS) -o $@

# release/ref_gl.so
release/ref_gl.so : $(OPENGL_OBJS)
	@echo '===> LD $@'
	${Q}$(CC) $(OPENGL_OBJS) $(LDFLAGS) $(X11LDFLAGS) -o $@

# release/baseq2/game.so
release/baseq2/game.so : $(GAME_OBJS)
	@echo '===> LD $@'
	${Q}$(CC) $(GAME_OBJS) $(LDFLAGS) -o $@

# ----------
