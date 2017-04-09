#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile).#
.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk image

# ------------------------------------------------------------------------------------
# Rules for buildStep: build and debug
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
image: /home/gombe/workspace/machikania/origdev/example/maze_withgene/bin/EXAMPLE1.elf nbproject/Makefile-default.mk 
else
image: /home/gombe/workspace/machikania/origdev/example/maze_withgene/bin/EXAMPLE1.hex nbproject/Makefile-default.mk 
endif

.PHONY: /home/gombe/workspace/machikania/origdev/example/maze_withgene/bin/EXAMPLE1.elf
/home/gombe/workspace/machikania/origdev/example/maze_withgene/bin/EXAMPLE1.elf: 
	cd . && cd ../&&make debug -j

.PHONY: /home/gombe/workspace/machikania/origdev/example/maze_withgene/bin/EXAMPLE1.hex
/home/gombe/workspace/machikania/origdev/example/maze_withgene/bin/EXAMPLE1.hex: 
	cd . && cd ../&&make -j


# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	cd . && cd ../&&make clean

