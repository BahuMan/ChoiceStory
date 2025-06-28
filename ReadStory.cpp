#include "ReadStory.hpp"



bool ReadStory::read(const char *filename) {
    if (lfs_file_open(&fs, &file, filename, LFS_O_RDONLY) != 0) {
        printf("Failed to open file %s\n", filename);
        finishedReading = true;
        return false;
    }
    printf("Reading story from %s\n", filename);
    idToPos.clear();
    char line[256];
    int n;
    std::string firstID("");
    while (true) {
        lfs_soff_t pos = lfs_file_tell(&fs, &file);
        n = ReadLine(&file, line, sizeof(line));
        if (n <= 0) break;
        // Remove newline
        char *newline = strchr(line, '\n');
        if (newline) *newline = '\0';
        if (strncmp(line, "ID=", 3) == 0) {
            std::string id = std::string(line + 3);
            if (firstID.empty()) {
                firstID = id; // store the first ID for jumping
            }
            idToPos[id] = pos;
            printf("Found ID: %s at position %ld\n", id.c_str(), pos);
        }
    }
    lfs_file_rewind(&fs, &file); // reset file pointer
    finishedReading = false;
    if (jump(firstID)) showStory(); // jump to the first story part
    badger.update();
    return true;
}

bool ReadStory::finished() {
    if (finishedReading) {
        return true; // already finished reading
    }

    bool screenChanged = false;
    if (badger.pressed(badger.A)) {
        screenChanged = true;
        finishedReading = !jump(optionAID); //finish on error
    }
    else if (badger.pressed(badger.B)) {
        screenChanged = true;
        finishedReading = !jump(optionBID); //finish on error
    }
    else if (badger.pressed(badger.C)) {
        screenChanged = true;
        finishedReading = !jump(optionCID); //finish on error
    }

    if (screenChanged) {
        sleep_ms(100); // debounce delay
        if (finishedReading) {
            printf("error -- aborting\n");
            sleep_ms(5000); // wait a bit before finishing
        }
        else {
            showStory();
        }
        badger.update();
    }


    return finishedReading; // still reading
}

bool ReadStory::showStory() {
    badger.text(currentStoryText, 0, 0, 2.0F);
    printf("Showing story text: %s\n", currentStoryText);
    badger.rectangle(0, 112, 296, 16); // clear the options area
    badger.pen(15); // set pen color to white for text
    badger.text(optionADisplay, 0, 112, 2.0F);
    badger.text(optionBDisplay, 100, 112, 2.0F);
    badger.text(optionCDisplay, 200, 112, 2.0F);
    badger.pen(0); // reset pen color to black for other text
    return true;
}
bool ReadStory::jump(std::string storyId) {
    // Clear buffers
    currentStoryText[0] = '\0';
    optionADisplay[0] = '\0';
    optionBDisplay[0] = '\0';
    optionCDisplay[0] = '\0';
    optionAID[0] = '\0';
    optionBID[0] = '\0';
    optionCID[0] = '\0';

    badger.update_speed(0);
    badger.font("bitmap8");
    badger.pen(15);
    badger.clear();
    badger.pen(0);

    // Find the position for the given ID
    auto it = idToPos.find(storyId);
    if (it == idToPos.end()) {
        badger.text(storyId + " not found", 0, 0, 2.0F);
        printf("Story ID %s not found\n", storyId.c_str());
        return false;
    }
    lfs_soff_t pos = it->second;
    lfs_file_seek(&fs, &file, pos, LFS_SEEK_SET);
    printf("Jumping to story ID: %s\n", storyId.c_str());

    char line[256];
    // Skip the ID= line
    if (ReadLine(&file, line, sizeof(line)) <= 0) {
        badger.text("Failed to read ID line for " + storyId, 0, 0, 2.0F);
        printf("Failed to read ID line for %s\n", storyId.c_str());
        return false;
    }

    // Read story text until we hit a line starting with A=
    int storyLen = 0;
    int n =ReadLine(&file, line, sizeof(line));
    while (n >= 0 && strncmp(line, "A=", 2) != 0) {

        // Append to story text
        int len = strlen(line);
        if (storyLen + len + 1 < (int)sizeof(currentStoryText)) {
            strcpy(currentStoryText + storyLen, line);
            storyLen += len;
            //currentStoryText[storyLen++] = '\n';
            currentStoryText[storyLen] = '\0';
        } else {
            badger.text(storyId + " text too long", 0, 0, 2.0F);
            printf("Story text too long for %s\n", storyId.c_str());
            return false;
        }
        
        n = ReadLine(&file, line, sizeof(line));
    }

    // Now read options A, B, C
    if (n < 0) {
        badger.text("No options found for " + storyId, 0, 0, 2.0F);
        printf("No options found for %s\n", storyId.c_str());
        return false;
    }
    //while reading story, we have already read the A= line
    if (!parseOption(line, "A=", optionADisplay, optionAID)) {
        badger.text("Invalid option A for " + storyId, 0, 16, 2.0F);
        printf("Invalid option A for %s\n", storyId.c_str());
        return false;
    }
    n = ReadLine(&file, line, sizeof(line));
    if (n<0) return true;
    if (!parseOption(line, "B=", optionBDisplay, optionBID)) {
        badger.text("Invalid option B for " + storyId, 0, 16, 2.0F);
        printf("Invalid option B for %s\n", storyId.c_str());
        return false;
    }
    n = ReadLine(&file, line, sizeof(line));
    if (n<0) return true;
    if (!parseOption(line, "C=", optionCDisplay, optionCID)) {
        badger.text("Invalid option C for " + storyId, 0, 16, 2.0F);
        return false;
    }
    return true;
}

bool ReadStory::parseOption(char* line, const char* const verify, char* displayBuffer, char* idBuffer) {
    if (strncmp(line, verify, strlen(verify)) != 0) {
        return false; // not a valid option line
    }

    char* sep = strchr(line + 2, '|');
    if (sep) {
        *sep = '\0';
        if (strnlen(line + 2, 60) > 59 || strnlen(sep + 1, 60) > 59) {
            badger.text("Option too long", 0, 0, 2.0F);
            printf("Option too long in line: %s\n", line);
            return false;
        }
        strncpy(displayBuffer, line + 2, 60);
        displayBuffer[60] = '\0'; // ensure null-termination
        strncpy(idBuffer, sep + 1, 60);
        //trim newline from idBuffer
        char* newline = strchr(idBuffer, '\n');
        if (newline) *newline = '\0'; // remove newline if present
        idBuffer[60] = '\0'; // ensure null-termination
        printf("Parsed option %s: %s -> %s\n", verify, displayBuffer, idBuffer);
        return true;
    } else {
        displayBuffer[0] = '\0';
        idBuffer[0] = '\0';
        return false;
    }
}
int ReadStory::ReadLine(lfs_file_t* file, char* buffer, int maxlen) {
    int i = 0;
    char c;
    while (i < maxlen - 1) {
        int n = lfs_file_read(&fs, file, &c, 1);
        if (n <= 0) break;
        buffer[i++] = c;
        if (c == '\n') break;
    }
    buffer[i] = '\0';
    return i;
}