// ui/display_lcd.cpp
#include "ui/display.h"
#include "drivers/lcdshield.h"


/* LCDのバックカラー
lcd.setBacklight(0);   // OFF
lcd.setBacklight(0x1); // 赤
lcd.setBacklight(0x2); // 緑
lcd.setBacklight(0x3); // 黄
lcd.setBacklight(0x4); // 青
lcd.setBacklight(0x5); // 紫
lcd.setBacklight(0x6); // 水
lcd.setBacklight(0x7); // 白
*/

/* 文字左スクロール
lcd.scrollDisplayLeft();
*/

void initDisplayLcd(){
    lcd.clear();
}

// バックカラー設定
void displayLcdSetBacklight(DisplayColor color)
{
    switch (color)
    {
        case DISP_OFF:   lcd.setBacklight(0x0); break;
        case DISP_RED:   lcd.setBacklight(0x1); break;
        case DISP_GREEN: lcd.setBacklight(0x2); break;
        case DISP_BLUE:  lcd.setBacklight(0x4); break;
        case DISP_WHITE: lcd.setBacklight(0x7); break;
    }
}

void displayLcdClear(){
    lcd.clear();
}

void displayLcdSetCursor(int col, int row){
    lcd.setCursor(col,row);
}

void displayLcdPrint(const char* text){
    lcd.print(text);
}

void displayLcdPrint(int value){
    lcd.print(value);
}

void displayLcdPrint(float value, int precision){
    lcd.print(value, precision);
}

void displayLcdWrite(uint8_t value){
    lcd.write(value);
}