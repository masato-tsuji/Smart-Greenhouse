#include "drivers/lcdshield.h"

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

void initLcdshield()
{
    lcd.begin(16, 2);
    lcd.setBacklight(0x7);
}