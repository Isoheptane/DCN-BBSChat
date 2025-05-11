#pragma once
#include "pch.h"

#include <windows.h>


namespace SetColor {
    void SetColor(WORD color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }
}
