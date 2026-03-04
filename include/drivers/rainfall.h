#pragma once
#include <arduino.h>

// 初期化（I2C/ライブラリ初期化）
void initRainfall();

// 累積降雨量(mm)を取得できたらtrue
bool rainfallReadTotalMm(float &out_total_mm);

// 直近の読み取りが成功しているか（品質）
bool rainfallOk();

// 最終成功時刻(ms)
uint32_t rainfallLastOkMs();

bool rainfallReadRawCount(uint32_t &out_count);