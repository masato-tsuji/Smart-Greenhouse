// menu/menu_actions.cpp
#include <Arduino.h>
#include "config/settings.h"
#include "menu/menu_actions.h"
#include "input/input.h"
#include "drivers/motor.h"
#include "actions/motor_control.h"
#include "ui/display.h"
#include "drivers/rainfall.h"
#include "actions/rainfall_state.h"

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


// ---- Rainfall Status 表示（追加） ----
#include "actions/rainfall_state.h"   // ★追加：rainfall_stateの値を参照

static const char* rainStateToStr(RainState st)
{
    switch (st)
    {
        case RainState::UNKNOWN: return "UNK";
        case RainState::RAINING: return "RAIN";
        case RainState::DRY:     return "DRY";
        default:                 return "???";
    }
}

void actionRainfallStatus()
{
    // 方針：
    // main で rainfallStateUpdate() を 500ms 周期で回しているならここは空でOK。
    // 画面表示中だけ更新したい場合は下を有効化（ただし二重更新になるならどちらかに統一）
    // rainfallStateUpdate(millis());
}

void drawRainfallStatus()
{
    const uint32_t now = millis();

    const RainState st = rainfallGetState();
    const bool ready = rainfallReady();

    // fresh判定：rainfallStateUpdate周期=500ms想定 → 2周期+αくらい
    const bool fresh = rainfallFresh(now, 1200);

    // 値：mmは整数表示でOK（小数点なし方針に合わせる）
    const int total_mm = (int)(rainfallGetLatestTotalMm());
    const int delta_mm = (int)(rainfallGetWindowDeltaMm());

    // 1行目：状態 + 累積
    displaySetCursor(0, 0);
    char line1[17];
    // 例: "R:RAIN T: 123mm"
    snprintf(line1, sizeof(line1), "R:%-4s T:%4dmm", rainStateToStr(st), total_mm);
    displayPrint(line1);

    // 2行目：窓内増分 + ready/fresh
    displaySetCursor(0, 1);
    char line2[17];
    // 例: "d: 12 r:1 f:1"
    snprintf(line2, sizeof(line2), "d:%3d r:%d f:%d", delta_mm, ready ? 1 : 0, fresh ? 1 : 0);
    displayPrint(line2);
}


