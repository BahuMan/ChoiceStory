#include <stdio.h>
#include "pico/stdlib.h"
#include "common/pimoroni_common.hpp"
#include "badger2040.hpp"

pimoroni::Badger2040 badger;

int main()
{
    stdio_init_all();
    badger.init();
    busy_wait_ms(1000);
    badger.update_speed(1);
    badger.font("bitmap6");
    badger.pen(15);
    badger.clear();
    badger.pen(0);
    badger.text("Init ChoiceStory v0.2", 0, 0, 2.0F);
    badger.update();

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
