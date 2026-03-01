#include <Arduino.h>
#include "menu/menu_actions.h"
#include "input/input.h"
#include "drivers/motor.h"

//void actionShowTempHumidity(){ Serial.println("Temp/Humi"); }
void actionControlSetup() {Serial.println("control");}
void actionSensorStatus(){ Serial.println("Sensor"); }
void actionViewLogs(){ Serial.println("Logs"); }
void actionTimeSetup(){ Serial.println("Time"); }
void actionAlarmStatus(){ Serial.println("Alarm"); }
void actionSystemInfo(){ Serial.println("System"); }

void actionMotorOpen() {Serial.println("Motor Open");}
void actionMotorClose() {Serial.println("Motor Close");}
void actionMotorManual() {

    static unsigned long last = 0;
    unsigned long now = millis();
    if (now - last > 500) { // 0.5秒に1回だけログ
        last = now;
        Serial.println("[actionMotorManual] alive");
    }
  
    InputButton btn = inputGetButton();   // 押されてるボタン（押してないなら BTN_NONE）
    motorManualUpdate(btn);
}


void drawSensorStatus() {}
void drawLogs() {}
void drawTimeSetup() {}
void drawSystemInfo() {}
void drawMotorOpen() {}
void drawMotorClose() {}
void drawMotorManual() {
    InputButton btn = inputGetButton();   // 押されてるボタン（押してないなら BTN_NONE）
    motorManualUpdate(btn);
}
