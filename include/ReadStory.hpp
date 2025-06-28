#pragma once
#include <stdlib.h>
#include <string.h>
#include <bsp/board.h>
#include <pico/stdlib.h>
#include <lfs.h>
#include <map>
#include <string>

#include "common/pimoroni_common.hpp"
#include "badger2040.hpp"

extern lfs_t fs;
extern pimoroni::Badger2040 badger;
//#define MIN(a, b) ((a) < (b) ? (a) : (b))

class ReadStory {
    public:
        //reset story, create index of story IDs and their positions in the file
        bool read(const char *filename);
        //process buttons and move to next story part
        //returns true if reading is finished, false if still reading
        bool finished();
        //returns false if jump failed due to syntax error or missing ID
        bool jump(std::string storyId);
        bool showStory();
    private:
        lfs_file_t file;
        bool finishedReading = true;
        // Maps story IDs to their positions in the file
        // This allows quick access to the position of a story by its ID
        std::map<std::string, lfs_soff_t> idToPos;
        int ReadLine(lfs_file_t* file, char* buffer, int maxlen);
        bool parseOption(char* line, const char* const verify, char* displayBuffer, char* idBuffer);
        char currentStoryText[512]; // buffer for the current story text
        char optionADisplay[64];
        char optionBDisplay[64];
        char optionCDisplay[64];
        char optionAID[64];
        char optionBID[64];
        char optionCID[64];
};
