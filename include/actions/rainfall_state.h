#pragma once
#include <arduino.h>

enum class RainState : uint8_t { UNKNOWN, RAINING, DRY };

// 初期化
void initRainfallState();

// 500ms周期で呼ぶ（main管理）
void rainfallStateUpdate(uint32_t now_ms);

// 状態取得
RainState rainfallGetState();
bool rainfallIsRaining();          // UNKNOWNは「雨扱い」に倒す（安全側）
bool rainfallReady();              // 初回の正常取得が完了したか
bool rainfallFresh(uint32_t now_ms, uint32_t fresh_timeout_ms);

// デバッグ用（任意）
float rainfallGetLatestTotalMm();
float rainfallGetWindowDeltaMm();