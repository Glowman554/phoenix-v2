cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)
include(pico-sdk/tools/CMakeLists.txt)

project(pico_emulator)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
set(TARGET pico_emulator)

add_executable(${TARGET}
    main.c
	programing.c
	debug.c
	readline.c
	setclock.c
)

include_directories(${TARGET} 
	${CMAKE_CURRENT_SOURCE_DIR}/../common/
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(${TARGET} PRIVATE
	-DSILENT
)

# pull in common dependencies
target_link_libraries(${TARGET} 
    pico_stdlib
)

# enable usb output & uart output
pico_enable_stdio_usb(${TARGET} 1)
pico_enable_stdio_uart(${TARGET} 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${TARGET})
