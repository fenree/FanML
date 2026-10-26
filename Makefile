APPDIR=${HOME}/pro/Particle/FanML
PARTICLE_CLI_PATH=/usr/bin/particle
PARTICLE_TOOL_COMPONENTS=workbench@1.16.51
PARTICLE_MAKEFILE=${HOME}/.particle/toolchains/buildscripts/1.17.2/Makefile
PARTICLE_DEVICE_ID=0a10aced202194944a059c4c
DEVICE_OS_VERSION=6.4.1
DEVICE_OS_PATH=${HOME}/.particle/toolchains/deviceOS/6.4.1
GCC_ARM_PATH=${HOME}/.particle/toolchains/gcc-arm/10.2.1/bin/
PLATFORM=p2
PLATFORM_ID=32
FC_FONTATIONS=1
OPENOCD_SCRIPTS=${HOME}/.particle/toolchains/openocd/0.12.0-particle.2/share/openocd/scripts
PATH=${HOME}/bin:${HOME}/.particle/toolchains/gcc-arm/10.2.1/bin:${HOME}/.particle/toolchains/buildtools/1.1.1:${HOME}/.particle/toolchains/openocd/0.12.0-particle.2/bin:/usr/local/sbin:/usr/local/bin:/usr/bin:/usr/bin/site_perl:/usr/bin/vendor_perl:/usr/bin/core_perl
export

.DEFAULT_GOAL := help
%:
	$(MAKE) -f $(PARTICLE_MAKEFILE) $@
