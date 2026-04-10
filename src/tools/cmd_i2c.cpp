#include <Arduino.h>
#include <Wire.h>

void toolCommandI2cScan()
{
    uint8_t foundCount = 0;

    Serial.println(F("[I2C SCAN] start"));

    for (uint8_t addr = 1; addr <= 0x7E; ++addr) {
        Wire.beginTransmission(addr);
        uint8_t error = Wire.endTransmission();

        if (error == 0) {
            Serial.print(F(" - found: 0x"));
            if (addr < 0x10) {
                Serial.print('0');
            }
            Serial.println(addr, HEX);
            ++foundCount;
        } else if (error == 4) {
            Serial.print(F(" - unknown error at 0x"));
            if (addr < 0x10) {
                Serial.print('0');
            }
            Serial.println(addr, HEX);
        }
    }

    Serial.print(F("[I2C SCAN] done. "));
    Serial.print(foundCount);
    Serial.println(F(" device(s)"));
}