#include "ChooseFile.hpp"


ChooseFile::ChooseFile(char **filenames, int nrFiles) {
    firstFileShown = 0;
    currentFile = 0;
    this->fileList = filenames;
    this->listSize = nrFiles;
    updateBadger();
}

//should be called continuously until returns non-null parameter
char * ChooseFile::chosen() {
    bool upPressed = badger.pressed(badger.UP);
    bool downPressed = badger.pressed(badger.DOWN);
    bool selectPressed = badger.pressed(badger.A) || badger.pressed(badger.B) || badger.pressed(badger.C);
    static int prevFile = 0;

    if (upPressed && currentFile > 0) {
        printf("up pressed\n");
        prevFile = currentFile;
        // If at the top of the window, scroll up by visibleFiles or to 0
        if (currentFile == firstFileShown) {
            int newFirst = firstFileShown - ChooseFile::visibleFiles;
            if (newFirst < 0) newFirst = 0;
            firstFileShown = newFirst;
            currentFile = firstFileShown;
            updateBadger();
        } else {
            currentFile--;
            moveArrow(prevFile, currentFile);
        }
    } else if (downPressed && currentFile < listSize - 1) {
        printf("down pressed\n");
        prevFile = currentFile;
        // If at the bottom of the window, scroll down by visibleFiles or to last window
        if (currentFile == firstFileShown + ChooseFile::visibleFiles - 1 || currentFile == listSize - 1) {
            int maxFirst = listSize - ChooseFile::visibleFiles;
            if (maxFirst < 0) maxFirst = 0;
            int newFirst = firstFileShown + ChooseFile::visibleFiles;
            if (newFirst > maxFirst) newFirst = maxFirst;
            firstFileShown = newFirst;
            currentFile = firstFileShown;
            updateBadger();
        } else {
            currentFile++;
            moveArrow(prevFile, currentFile);
        }
    }

    if (selectPressed) {
        printf("select %s pressed\n", fileList[currentFile]);
        return fileList[currentFile];
    }
    return nullptr; // no file chosen yet
}

void ChooseFile::updateBadger() {
    badger.update_speed(0);
    badger.font("bitmap8");
    badger.pen(15);
    badger.clear();
    badger.pen(0);
    for (int f=firstFileShown; f<listSize && f<firstFileShown+ChooseFile::visibleFiles; f++) {
        badger.text(fileList[f], 20, (f-firstFileShown)*16, 2.0F);
    }
    drawArrow(0, (currentFile-firstFileShown)*16);
    badger.update();
}
void ChooseFile::moveArrow(int prevIndex, int newIndex)
{
    badger.update_speed(3); //fast update for responsive arrow movement
    if (prevIndex == newIndex) return; // no change
    int y = (newIndex-firstFileShown)*16;
    badger.pen(15);
    badger.rectangle(0, prevIndex*16, 10, 16); // clear previous arrow
    drawArrow(0, y);
    badger.partial_update(0, 0, 10, 128); // leave file list intact
}

void ChooseFile::drawArrow(int x, int y) {
    badger.pen(0);
    badger.line(x, y,    x+10, y+8);
    badger.line(x, y+8,  x+10, y+8);
    badger.line(x, y+16, x+10, y+8);
}
