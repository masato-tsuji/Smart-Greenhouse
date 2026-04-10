#include "actions/motor_control.h"
#include "drivers/motor.h"
#include "drivers/sensor.h"
#include "input/input.h"
#include "config/settings.h"
#include "actions/rainfall_state.h"
// #include "ui/display.h"

//====================================================
// モード
//====================================================
static MotorControlMode g_mode = MODE_STOP;

void motorSetMode(MotorControlMode mode)
{
    g_mode = mode;

    // STOPへ入れた瞬間は確実に止める（安全）
    if (g_mode == MODE_STOP)
    {
        motorCommand(MOTOR_CMD_STOP);
    }
}

MotorControlMode motorGetMode()
{
    return g_mode;
}

//====================================================
// LS状態取得
//====================================================



//====================================================
// 手動制御（ボタン押下中だけ動作）
//====================================================
static void motorManualControl()
{
    InputButton btn = inputGetButton();

    if (btn == BTN_UP)
    {
        motorCommand(MOTOR_CMD_OPEN);
    }
    else if (btn == BTN_DOWN)
    {
        motorCommand(MOTOR_CMD_CLOSE);
    }
    else
    {
        motorCommand(MOTOR_CMD_STOP);
    }
}

//====================================================
// 自動制御（温度ベース）
// - 基本：
//   t >= openTemp  → OPEN
//   t <= closeTemp → CLOSE
//   それ以外       → STOP
//====================================================
static void motorAutoControl()
{
    float t = readTemperature();

    // 設定値は config/settings 側に持たせる前提
    // tempSettings.openTemp, tempSettings.closeTemp
    if (t >= (float)tempSettings.openTemp)
    {
        motorCommand(MOTOR_CMD_OPEN);
    }
    else if (t <= (float)tempSettings.closeTemp)
    {
        motorCommand(MOTOR_CMD_CLOSE);
    }
    else
    {
        motorCommand(MOTOR_CMD_STOP);
    }
}

MotorStatus motorGetStatus()
{
    MotorStatus s;
    s.dir = motorGetDir();
    // 以下実装中
    // s.pwm = motorGetCurrentPwm();
    // s.upperLimit = motorIsUpperLimit();
    // s.lowerLimit = motorIsLowerLimit();
    return s;
}


//====================================================
// 周期更新（呼び出し側で refreshMs を決める）
//====================================================

// 例：rainfallStateUpdate を 500ms で呼んでいるなら、fresh許容は2倍くらいが妥当
static constexpr uint32_t RAIN_FRESH_TIMEOUT_MS = 1200; // 500ms×2 + α

void motorControlUpdate(uint32_t now_ms)
{
    switch (g_mode)
    {
        case MODE_MANUAL:
            motorManualControl();
            break;

        case MODE_AUTO:
        {
            // --- 1) 起動直後/未取得：勝手に動かさない ---
            if (!rainfallReady())
            {
                motorCommand(MOTOR_CMD_STOP);
                break;
            }

            // --- 2) センサー更新が止まった：安全側で閉める（開けない） ---
            if (!rainfallFresh(now_ms, RAIN_FRESH_TIMEOUT_MS))
            {
                motorCommand(MOTOR_CMD_CLOSE);
                break;
            }

            // --- 3) 雨なら即閉める ---
            if (rainfallIsRaining())
            {
                motorCommand(MOTOR_CMD_CLOSE);
                break;
            }

            // --- 4) 晴れ：通常の自動制御（温度ANDなど） ---
            motorAutoControl();
            break;
        }

        case MODE_STOP:
        default:
            motorCommand(MOTOR_CMD_STOP);
            break;
    }
}


