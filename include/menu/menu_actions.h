#pragma once

// ===== 温湿度表示 =====
void actionShowTempHumidity();

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