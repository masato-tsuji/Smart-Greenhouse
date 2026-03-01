#pragma once
#include <Arduino.h>
#include "ui/display.h"

void initDisplayLcd();
void displayLcdSetBacklight(DisplayColor color);
void displayLcdClear();
void displayLcdSetCursor(int col, int row);
void displayLcdPrint(const char* text);
void displayLcdPrint(int value);
void displayLcdPrint(float value, int digits);
void displayLcdWrite(uint8_t value);
