# Choose Your Own Adventure for Badger2040

Small text-adventure engine, where a little bit of text is presented
on the badger's e-ink screen, and then 3 choices are presented
using the bottom 3 buttons. Each of the buttons jumps to a different
part of the story.

The idea is to read the story from a file in flash memory.
When the badger is plugged into a PC, this file can be edited with
any text editor to create extra chapters in the adventure.

## The badge:  
[https://shop.pimoroni.com/products/badger-2040]

## Building the project:
after cloning this project, use git to retrieve littlefs submodule:  
`git submodule update --init`
 
This is a VSCode project. You need to have the Raspberry Pi Pico extension installed.
It will download the pico-sdk for you.
When you open the project folder, give the extension some time to install the pico-sdk.
At the bottom right of the screen, you should find a small button to **compile** or to **run**.
If the code does not build, check the [CMakeLists.txt](CMakeLists.txt) file and make sure paths are correct.
There may be a difference between Linux and Windows.


## Creating a story
When you first start the badge, it will present itself as a USB
stick with a file called "MANUAL.TXT". This file explains the syntax
but is also written as an example story with menu options that
you can read as if it were a story.
