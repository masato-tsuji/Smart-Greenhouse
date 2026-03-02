// menu/menu_actions.cpp
#include <Arduino.h>
#include "config/settings.h"
#include "menu/menu_actions.h"
#include "input/input.h"
#include "drivers/motor.h"
#include "actions/motor_control.h"
#include "ui/display.h"

//void actionShowTempHumidity(){ Serial.println("Temp/Humi"); }

static uint8_t g_controlSel = 0;   // 0=open, 1=close
static bool    g_editMode   = false;


// 動作温度設定

void actionControlSetup(){
    InputEvent ev = inputGetEvent();

    if (ev.type == INPUT_NONE) return;

    // ---- 編集モードでない ----
    if (!g_editMode)
    {
        if (ev.type == INPUT_PRESS || ev.type == INPUT_REPEAT)
        {
            if (ev.button == BTN_UP || ev.button == BTN_DOWN)
            {
                g_controlSel = (g_controlSel == 0) ? 1 : 0;
            }
        }

        if (ev.type == INPUT_PRESS && ev.button == BTN_RIGHT)
        {
            g_editMode = true;  // 編集開始
        }
    }
    // ---- 編集モード ----
    else
    {
        if (ev.type == INPUT_PRESS || ev.type == INPUT_REPEAT)
        {
            if (ev.button == BTN_UP)
            {
                if (g_controlSel == 0)
                    tempSettings.openTemp++;
                else
                    tempSettings.closeTemp++;
            }

            if (ev.button == BTN_DOWN)
            {
                if (g_controlSel == 0)
                    tempSettings.openTemp--;
                else
                    tempSettings.closeTemp--;
            }
        }

        if (ev.type == INPUT_PRESS && ev.button == BTN_RIGHT)
        {
            g_editMode = false;  // 編集確定
        }

        if (ev.type == INPUT_PRESS && ev.button == BTN_LEFT)
        {
            g_editMode = false;  // キャンセル扱い（今回は値保持のまま）
        }
    }
}
void actionSensorStatus(){}
void actionViewLogs(){}
void actionTimeSetup(){}
void actionAlarmStatus(){}
void actionSystemInfo(){}

void actionMotorOpen(){}
void actionMotorClose(){}

// motor_controlで実装しているためここでは処理しない
void actionMotorManual(){}

// 温度設定表示
void drawControlSetup()
{
    displaySetCursor(0, 0);
    displayPrint(g_controlSel == 0 ? ">" : " ");
    displayPrint("Open :");

    char buf[6];
    snprintf(buf, sizeof(buf), "%3d", tempSettings.openTemp);
    displayPrint(buf);

    displaySetCursor(0, 1);
    displayPrint(g_controlSel == 1 ? ">" : " ");
    displayPrint("Close:");

    snprintf(buf, sizeof(buf), "%3d", tempSettings.closeTemp);
    displayPrint(buf);
}

// LSの状態を返す
static const char* toHl(uint8_t v)
{
    return (v == HIGH) ? "HIGH" : "LOW ";
}
void drawSensorStatus() {
    displaySetCursor(0,0);
    displayPrint("U:");
    displayPrint(toHl(motorGetUpperLsRaw()));
    
    displaySetCursor(0,1);
    displayPrint("L:");
    displayPrint(toHl(motorGetLowerLsRaw()));
}

void drawLogs() {}
void drawTimeSetup() {}
void drawSystemInfo() {}
void drawMotorOpen() {}
void drawMotorClose() {}

// MOTORの手動操作表示
void drawMotorManual() {
    MotorStatus s = motorGetStatus();

    displaySetCursor(0,0);
    displayPrint("UP/DWN KEY PRESS");

    displaySetCursor(0,1);
    if (s.dir == 1) displayPrint("UP  ");
    else if (s.dir == 2) displayPrint("DOWN ");
    else displayPrint("STOP ");    
}
