#pragma once
#include "pch.h"
#include "SetColor.h"
#include <windows.h>



namespace SetColor {
    void setColor(WORD color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }
}
