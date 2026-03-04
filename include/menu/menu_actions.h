// menu/menu_actions.h
#pragma once

// ===== 温湿度表示 =====
void actionShowTempHumidity();


// ===== 降雨センサー表示 =====
void actionRainfallStatus();
void drawRainfallStatus();


void actionControlSetup();
void actionManualControl();
void actionSensorStatus();
void actionViewLogs();
void actionTimeSetup();
void actionAlarmStatus();
void actionSystemInfo();

void actionMotorOpen();
void actionMotorClose();
void actionMotorManual();

// ===== DRAW =====
void drawTempHumidity();
void drawSensorStatus();
void drawLogs();
void drawTimeSetup();
void drawSystemInfo();
void drawMotorOpen();
void drawMotorClose();
void drawMotorManual();