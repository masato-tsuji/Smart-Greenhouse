#include "drivers/lcdshield.h"

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

void initLcdshield()
{
    lcd.begin(20, 4);
    lcd.setBacklight(0x7);
}