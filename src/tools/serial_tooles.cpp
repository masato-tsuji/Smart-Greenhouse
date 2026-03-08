#include "tools/serial_tools.h"


#include <Arduino.h>

// cmd_i2c.cpp 側の関数
void toolCommandI2cScan();

namespace
{
    static char g_lineBuf[32];
    static uint8_t g_lineLen = 0;

    void dispatchCommand(const char* cmd)
    {
        if (strcmp(cmd, "i2c") == 0) {
            toolCommandI2cScan();
            return;
        }

        if (strcmp(cmd, "status") == 0) {
            Serial.println(F("SYSTEM OK"));
            return;
        }

        if (strcmp(cmd, "help") == 0) {
            Serial.println(F("Commands:"));
            Serial.println(F("  help  - show commands"));
            Serial.println(F("  i2c   - scan I2C addresses"));
            return;
        }

        if (cmd[0] != '\0') {
            Serial.print(F("Unknown command: "));
            Serial.println(cmd);
            Serial.println(F("Type 'help'"));
        }
    }
}

void initSerialTools()
{
    g_lineLen = 0;
    g_lineBuf[0] = '\0';
}

void serialToolsUpdate()
{
    while (Serial.available() > 0) {
        char c = static_cast<char>(Serial.read());

        // CRは無視
        if (c == '\r') {
            continue;
        }

        // LFで1コマンド確定
        if (c == '\n') {
            g_lineBuf[g_lineLen] = '\0';
            dispatchCommand(g_lineBuf);

            g_lineLen = 0;
            g_lineBuf[0] = '\0';
            continue;
        }

        // バッファに余裕があれば追加
        if (g_lineLen < sizeof(g_lineBuf) - 1) {
            g_lineBuf[g_lineLen++] = c;
        }
    }
}