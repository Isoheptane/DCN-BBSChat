#pragma once
#include "pch.h"
#include "SetColor.h"
#include <windows.h>

#include <string>
using std::string;

namespace SetColor {
    void setColor(WORD color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }

    Color genColor(string name) {
        const static Color colors[] = {
            Color::RED,
            Color::YELLOW,
            Color::GREEN,
            Color::CYAN,
            Color::BLUE,
            Color::PURPLE,
            Color::LIGHT_RED,
            Color::LIGHT_YELLOW,
            Color::LIGHT_GREEN,
            Color::LIGHT_CYAN,
            Color::LIGHT_BLUE,
            Color::LIGHT_PURPLE,
        };

        int vector = 0;
        for (char ch : name) {
            vector = (vector + ch) % 12;
        }

        return colors[vector];
    }
}
