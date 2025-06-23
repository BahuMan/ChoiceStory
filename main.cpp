/*
 * Raspberry Pi Pico littlefs USB Flash Memory Interface
 *
 * 
 * Copyright 2024, Hiroyuki OYAMA. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdlib.h>
#include <string.h>
#include <bsp/board.h>
#include <pico/stdlib.h>
#include <tusb.h>
#include <lfs.h>
#include "bootsel_button.h"
extern "C" {
#include "mimic_fat.h"
}
#include "common/pimoroni_common.hpp"
#include "badger2040.hpp"

extern const struct lfs_config lfs_pico_flash_config;  // littlefs_driver.c

#define FILENAME  "SENSOR.TXT"

#include "defaultstory.h"

#define ANSI_RED "\e[31m"
#define ANSI_CLEAR "\e[0m"


lfs_t fs;

/*
 * Format the file system if it does not exist
 */
static void test_filesystem_and_format_if_necessary(bool force_format) {
   if (force_format || (lfs_mount(&fs, &lfs_pico_flash_config) != 0)) {
        printf("Format the onboard flash memory with littlefs\n");

        lfs_format(&fs, &lfs_pico_flash_config);
        lfs_mount(&fs, &lfs_pico_flash_config);

        lfs_file_t f;
        lfs_file_open(&fs, &f, "MANUAL.TXT", LFS_O_RDWR|LFS_O_CREAT);
        lfs_file_write(&fs, &f, DEFAULT_STORY, strlen(DEFAULT_STORY));
        lfs_file_close(&fs, &f);

        if (mimic_fat_usb_device_is_enabled()) {
            mimic_fat_create_cache();
        }
    }
}

/*
 * Log clicks on the BOOTSEL button to a log file.
 * Press and hold the button for 10 seconds to initialize the file system.
 */
static void sensor_logging_task(void) {
    static bool last_status = false;
    static int count = 0;
    bool button = bb_get_bootsel_button();
    static uint64_t long_push = 0;

    if (last_status != button && button) {  // Push BOOTSEL button
        count += 1;
        printf("Update %s\n", FILENAME);

        lfs_file_t f;
        lfs_file_open(&fs, &f, FILENAME, LFS_O_RDWR|LFS_O_APPEND|LFS_O_CREAT);
        uint8_t buffer[512];
        snprintf((char *)buffer, sizeof(buffer), "click=%d\n", count);
        lfs_file_write(&fs, &f, buffer, strlen((char *)buffer));
        printf((char *)buffer);
        lfs_file_close(&fs, &f);
    }
    last_status = button;

    if (button) {
        long_push++;
    } else {
        long_push = 0;
    }
    if (long_push > 35000) { // Long-push BOOTSEL button
        test_filesystem_and_format_if_necessary(true);
        count = 0;
        long_push = 0;
    }
}
pimoroni::Badger2040 badger;

int main(void) {
    //set_sys_clock_khz(250000, false);

    board_init();
    tud_init(BOARD_TUD_RHPORT);
    stdio_init_all();
    badger.init();

    test_filesystem_and_format_if_necessary(false);

    badger.update_speed(1);
    badger.font("bitmap8");
    badger.pen(15);
    badger.clear();
    badger.pen(0);
    badger.text("Init ChoiceStory v0.6", 0, 0, 1.0F);
    lfs_dir_t dir;
    if (lfs_dir_open(&fs, &dir, "/") < 0) {
        badger.text("error opening root", 0, 20, 1.0F);
    }
    else {
        lfs_info info;
        int y = 20;
        while (lfs_dir_read(&fs, &dir, &info) > 0) {
            badger.text(info.name, 10, y, 1.0F);
            y += 8;
        }
    }
    badger.update();

    while (true) {
        sensor_logging_task();
        tud_task();
    }
}
