#pragma once

#include <windows.h>

namespace SetColor {
    // set the color of the word
    void setColor(WORD color);

    // Common color definitions
    enum Color {
        BLACK = 0,
        BLUE = 1,
        GREEN = 2,
        CYAN = 3,
        RED = 4,
        PURPLE = 5,
        YELLOW = 6,
        WHITE = 7,
        GRAY = 8,
        LIGHT_BLUE = 9,
        LIGHT_GREEN = 10,
        LIGHT_CYAN = 11,
        LIGHT_RED = 12,
        LIGHT_PURPLE = 13,
        LIGHT_YELLOW = 14,
        BRIGHT_WHITE = 15
    };
}
#pragma once
