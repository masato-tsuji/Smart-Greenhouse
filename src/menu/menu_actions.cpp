#include <Arduino.h>
#include "menu/menu_actions.h"
#include "input/input.h"
// #include "drivers/motor.h"
#include "actions/motor_control.h"
#include "ui/display.h"

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
  
    // InputButton btn = inputGetButton();   // 押されてるボタン（押してないなら BTN_NONE）
    // motorManualUpdate(btn);
}


void drawSensorStatus() {}
void drawLogs() {}
void drawTimeSetup() {}
void drawSystemInfo() {}
void drawMotorOpen() {}
void drawMotorClose() {}
void drawMotorManual() {
    MotorStatus s = motorGetStatus();

    displaySetCursor(0,0);
    displayPrint("UP/DWN KEY PRESS");

    displaySetCursor(0,1);
    if (s.dir == 1) displayPrint("UP  ");
    else if (s.dir == 2) displayPrint("DOWN ");
    else displayPrint("STOP ");    
}
