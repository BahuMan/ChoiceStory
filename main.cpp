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
#include "ChooseFile.hpp"
extern const struct lfs_config lfs_pico_flash_config;  // littlefs_driver.c

#include "defaultstory.h"

#define ANSI_RED "\e[31m"
#define ANSI_CLEAR "\e[0m"


lfs_t fs;
pimoroni::Badger2040 badger;

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
    bool button = bb_get_bootsel_button();
    static uint64_t long_push = 0;

    if (last_status != button && button) {  // Push BOOTSEL button
        //this was where the counter was written to file SENSOR.TXT
        printf("BOOT pressed");
    }
    last_status = button;

    if (button) {
        long_push++;
    } else {
        long_push = 0;
    }
    if (long_push > 35000) { // Long-push BOOTSEL button
        test_filesystem_and_format_if_necessary(true);
        long_push = 0;
    }
}

enum activity {
    CHOOSING_FILE,
    READING_STORY
};

static activity current_activity = CHOOSING_FILE;
int main(void) {
    //set_sys_clock_khz(250000, false);

    board_init();
    tud_init(BOARD_TUD_RHPORT);
    stdio_init_all();
    badger.init();

    test_filesystem_and_format_if_necessary(false);

    badger.update_speed(0);
    badger.font("bitmap8");
    badger.pen(15);
    badger.clear();
    badger.pen(0);
    badger.text("Init ChoiceStory v0.7", 0, 0, 2.0F);
    lfs_dir_t dir;
    int file_count = 0;
    if (lfs_dir_open(&fs, &dir, "/") >= 0) {
        lfs_info info;
        // First pass: count files
        while (lfs_dir_read(&fs, &dir, &info) > 0) {
            if (strcmp(info.name, "..") == 0) continue;
            if (strcmp(info.name, ".") == 0) continue;
            file_count++;
        }
        lfs_dir_close(&fs, &dir);
    }
    // Allocate filelist
    char **filelist = nullptr;
    if (file_count > 0) {
        filelist = (char**)malloc(sizeof(char*) * file_count);
        lfs_dir_open(&fs, &dir, "/");
        lfs_info info;
        int idx = 0;
        while (lfs_dir_read(&fs, &dir, &info) > 0 && idx < file_count) {
            if (strcmp(info.name, "..") == 0) continue;
            if (strcmp(info.name, ".") == 0) continue;
            filelist[idx] = strdup(info.name);
            idx++;
        }
        lfs_dir_close(&fs, &dir);
    }
    ChooseFile chooser(filelist, file_count);
    badger.update();

    while (true) {
        sensor_logging_task();
        tud_task();
        char *chosen_file = nullptr;
        badger.update_button_states();
        switch (current_activity)
        {
        case CHOOSING_FILE:
            chosen_file = chooser.chosen();
            if (chosen_file) {
                current_activity = READING_STORY;
            }
            break;
        
        default:
            break;
        }
    }
}
