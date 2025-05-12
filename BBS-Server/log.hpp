#pragma once

#ifndef __LOG_HPP
#define __LOG_HPP

#include <ctime>
#include <cstdio>
#include <cstdarg>
#include <SetColor.h>

using SetColor::setColor;
using SetColor::Color;

void printLog(const char* type, SetColor::Color color, const char* format, va_list args) {

    time_t t = time(NULL);
    tm local;
    localtime_s(&local, &t);

    printf("[%02d:%02d:%02d] ", local.tm_hour, local.tm_min, local.tm_sec);

    setColor(Color::WHITE);
    printf("[");
    setColor(color);
    printf("%s", type);
    setColor(Color::WHITE);
    printf("] ");

    vprintf(format, args);
}

void info(const char* format, ... ) {
    va_list args;
    va_start(args, format);
    printLog("INFO", Color::LIGHT_GREEN, format, args);
    va_end(args);
}

void warn(const char* format, ...) {
    va_list args;
    va_start(args, format);
    printLog("INFO", Color::LIGHT_YELLOW, format, args);
    va_end(args);
}

void error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    printLog("ERROR", Color::LIGHT_RED, format, args);
    va_end(args);
}

#endif