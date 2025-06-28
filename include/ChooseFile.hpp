#pragma once

#include <pico/stdlib.h>
#include <string.h>
#include "common/pimoroni_common.hpp"
#include "badger2040.hpp"

extern pimoroni::Badger2040 badger;

class ChooseFile {
    public:
        static constexpr int visibleFiles = 128 / 16;
        ChooseFile(char** filenames, int nrFiles);
        char *chosen();
    private:
        char **fileList;
        int listSize;
        int firstFileShown;
        int currentFile;
        void updateBadger();
        void moveArrow(int prevIndex, int newIndex);
        void drawArrow(int x, int y);
};