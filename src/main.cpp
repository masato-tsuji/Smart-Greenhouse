// main.cpp (entrypoint)

#include "drivers/sensor.h"
#include "menu/menu.h"
#include "ui/display.h"
#include "input/input.h"
#include "drivers/motor.h"


#include "actions/motor_control.h"

static unsigned long lastControl = 0;
static constexpr unsigned long CONTROL_PERIOD_MS = 20;

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
    // Menu更新ループ
    menuUpdate();

    // モーター制御ループ
    unsigned long now = millis();
    if (now - lastControl >= CONTROL_PERIOD_MS)
    {
        lastControl = now;
        motorControlUpdate();
    }

}

