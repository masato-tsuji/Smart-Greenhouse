// ui/display.h
#pragma once
#include <Arduino.h>

enum DisplayColor {
    DISP_OFF,
    DISP_RED,
    DISP_GREEN,
    DISP_BLUE,
    DISP_WHITE
};


void initDisplay();
void displaySetBacklight(DisplayColor color);
void displayBacklightOff();
void displayClear();
void displaySetCursor(int col, int row);
void displayPrint(const char* text);
void displayPrint(int value);
void displayPrint(float value, int digits);
void displayWrite(uint8_t value);