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



struct RainfallStateSettings {
  float    amount_mm;        // 雨判定量（mm）
  uint32_t rain_window_sec;  // 雨判定時間（秒）
  uint32_t dry_time_sec;     // 晴れ判定時間（秒）
};

extern RainfallStateSettings rainfallStateSettings;

// 既存の motorSettings などもここにある想定