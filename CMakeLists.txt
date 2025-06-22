# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(FAMILY rp2040)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(ChoiceStory C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/vendor/badger2040)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/vendor/hiroyuki)

add_executable(ChoiceStory
  main.cpp
)

target_link_libraries(ChoiceStory PRIVATE
  hardware_flash
  hardware_sync
  littlefs
  pico_stdlib
  tinyusb_additions
  tinyusb_board
  tinyusb_device
  badger2040
  hiroyuki
)

target_include_directories(ChoiceStory
  PRIVATE
  vendor/littlefs
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/vendor/badger2040
)
target_compile_options(ChoiceStory PRIVATE -Werror -Wall -Wextra -Wnull-dereference)
target_link_options(ChoiceStory PRIVATE -Wl,--print-memory-usage)

pico_enable_stdio_usb(ChoiceStory 1)
pico_add_extra_outputs(ChoiceStory)

pico_add_extra_outputs(ChoiceStory)

