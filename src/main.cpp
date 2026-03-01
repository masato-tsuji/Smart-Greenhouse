// main.cpp (entrypoint)

#include "drivers/sensor.h"
#include "menu/menu.h"
#include "ui/display.h"
#include "input/input.h"
#include "drivers/motor.h"

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

    // InputButton btn = inputGetButton();
    // motorManualUpdate(btn);
    // delay(5);   // 少しだけ落ち着かせる（なくてもOK）
}

