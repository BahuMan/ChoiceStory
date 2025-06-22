
add_library(badger2040)

target_sources(badger2040 PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/badger2040.cpp
  ${CMAKE_CURRENT_LIST_DIR}/common/pimoroni_bus.cpp
  ${CMAKE_CURRENT_LIST_DIR}/common/pimoroni_i2c.cpp
  ${CMAKE_CURRENT_LIST_DIR}/drivers/uc8151_legacy/uc8151_legacy.cpp
  ${CMAKE_CURRENT_LIST_DIR}/libraries/bitmap_fonts/bitmap_fonts.cpp
  ${CMAKE_CURRENT_LIST_DIR}/libraries/hershey_fonts/hershey_fonts_data.cpp
  ${CMAKE_CURRENT_LIST_DIR}/libraries/hershey_fonts/hershey_fonts.cpp
)

# Add any user requested libraries
target_link_libraries(badger2040 pico_stdlib hardware_spi hardware_pwm hardware_i2c)

target_include_directories(badger2040 PUBLIC ${CMAKE_CURRENT_LIST_DIR})