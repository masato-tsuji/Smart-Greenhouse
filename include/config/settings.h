// config/setting.h
// 外部から設定する項目をここにまとめる
#pragma once
#include <Arduino.h>

// extern int openTemp;
// extern int closeTemp;

struct TempSettings
{
    int openTemp;
    int closeTemp;
};
extern TempSettings tempSettings;


struct MotorSettings
{
    uint8_t  limitPercent;   // 0-100
    uint16_t rampTimeMs;     // ms
};
extern MotorSettings motorSettings;