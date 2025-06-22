add_library(hiroyuki)

target_sources(hiroyuki PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/littlefs_driver.c
    ${CMAKE_CURRENT_LIST_DIR}/mimic_fat.c
    ${CMAKE_CURRENT_LIST_DIR}/unicode.c
    ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
    ${CMAKE_CURRENT_LIST_DIR}/usb_msc_driver.c
)

add_library(littlefs INTERFACE)
target_sources(littlefs INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/../littlefs/lfs.c
  ${CMAKE_CURRENT_LIST_DIR}/../littlefs/lfs_util.c
)

target_link_libraries(hiroyuki
    pico_stdlib
    hardware_spi
    hardware_pwm
    hardware_i2c
    tinyusb_additions
    littlefs
    tinyusb_board
    tinyusb_device
)

target_include_directories(hiroyuki PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../../include
    ${CMAKE_CURRENT_LIST_DIR}/../littlefs
)

target_compile_options(littlefs INTERFACE -Wno-unused-function -Wno-null-dereference)
target_link_libraries(littlefs INTERFACE hardware_flash)
