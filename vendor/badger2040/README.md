# C++ library for the Badger2040

This folder can be added to a standard pico c++ sdk project.

The code I based this on:
[https://github.com/pimoroni/pimoroni-pico/tree/main/examples/badger2040]

Looking through the code, I needed to cobble together 4 different parts of the Pimoroni repo:
1. drivers/uc8151_legacy
2. libraries/bitmap_fonts
3. libraries/hershey_fonts
4. common

In your root CMakeLists.txt, add the folling lines:
```
add_subdirectory(badger2040)
target_link_libraries(ChoiceStory
        badger2040)
```
