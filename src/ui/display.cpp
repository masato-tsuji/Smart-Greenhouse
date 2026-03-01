// ui/display.cpp
#include "ui/display.h"
#include "ui/display_lcd.h"

// 実装を選択
void initDisplay(){
    initDisplayLcd();
}

void displaySetBacklight(DisplayColor color){
    displayLcdSetBacklight(color);
}

void displayClear(){
    displayLcdClear();
}

void displaySetCursor(int col, int row){
    displayLcdSetCursor(col,row);
}

void displayPrint(const char* text){
    displayLcdPrint(text);
}

void displayPrint(int value){
    displayLcdPrint(value);
}

void displayPrint(float value, int digits){
  displayLcdPrint(value, digits);
}

void displayWrite(uint8_t value){
    displayLcdWrite(value);
}