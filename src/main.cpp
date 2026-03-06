// main.cpp (entry point)

#include "drivers/sensor.h"
#include "menu/menu.h"
#include "ui/display.h"
#include "input/input.h"
#include "drivers/motor.h"
#include "actions/motor_control.h"
#include "actions/rainfall_state.h"

// 制御周期と雨量更新周期を定義
static unsigned long lastControl = 0;
static constexpr unsigned long CONTROL_PERIOD_MS = 20;

static unsigned long lastRain = 0;
static constexpr unsigned long RAIN_PERIOD_MS = 500;

// 初期化とメインループ
void setup()
{

    // debug用
    Serial.begin(115200);

    // 初期化
    initDisplay();  // 表示ユニットLCD
    initInput();    // 表示ユニットボタン（何もしてないけど何となく）
    initSensor();   // 温湿度センサー
    initMotor();    // モーター
    initMenu();     // Menu

}

void loop()
{
    menuUpdate();

    unsigned long now = millis();

    // 1) rainfallを先に（可能なら）
    if (now - lastRain >= RAIN_PERIOD_MS)
    {
        lastRain += RAIN_PERIOD_MS;
        rainfallStateUpdate(now);
    }

    // 2) motor_control
    if (now - lastControl >= CONTROL_PERIOD_MS)
    {
        lastControl += CONTROL_PERIOD_MS;
        motorControlUpdate(now);
    }
}

