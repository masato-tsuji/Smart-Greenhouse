#include "actions/rainfall_state.h"
#include "drivers/rainfall.h"
#include "config/settings.h"     // ここに3パラメータを置く想定
#include <Arduino.h>
#include <deque>

struct Sample {
  uint32_t t_ms;
  float total_mm;
};

static RainState g_state = RainState::UNKNOWN;
static bool g_ready = false;
static uint32_t g_last_update_ms = 0;

static float g_last_total_mm = 0.0f;
static float g_window_delta_mm = 0.0f;
static uint32_t g_last_rain_ms = 0;

static std::deque<Sample> g_samples;

void initRainfallState()
{
  g_state = RainState::UNKNOWN;
  g_ready = false;
  g_last_update_ms = 0;
  g_last_total_mm = 0;
  g_window_delta_mm = 0;
  g_last_rain_ms = 0;
  g_samples.clear();
}

RainState rainfallGetState() { return g_state; }

bool rainfallReady() { return g_ready; }

bool rainfallFresh(uint32_t now_ms, uint32_t fresh_timeout_ms)
{
  if (!g_ready) return false;
  return (now_ms - g_last_update_ms) <= fresh_timeout_ms;
}

bool rainfallIsRaining()
{
  // UNKNOWNは安全側で雨扱い（※ただし motor_control 側で ready まで動かさない運用が相性良い）
  return (g_state == RainState::RAINING) || (g_state == RainState::UNKNOWN);
}

float rainfallGetLatestTotalMm() { return g_last_total_mm; }
float rainfallGetWindowDeltaMm() { return g_window_delta_mm; }

void rainfallStateUpdate(uint32_t now_ms)
{
  float total_mm;
  if (!rainfallReadTotalMm(total_mm))
  {
    // 読めない → UNKNOWN
    g_state = RainState::UNKNOWN;
    return;
  }

  g_ready = true;
  g_last_update_ms = now_ms;

  // センサー累積がリセットされた（停電/瞬断など）を検知
  if (!g_samples.empty() && total_mm < g_last_total_mm)
  {
    g_samples.clear();
    g_state = RainState::UNKNOWN;
    g_last_rain_ms = 0;
  }

  g_last_total_mm = total_mm;

  // 履歴へ追加
  g_samples.push_back({now_ms, total_mm});

  // 観測窓の外を削除
  const uint32_t win = rainfallStateSettings.rain_window_sec * 1000UL; // ②
  while (!g_samples.empty() && (now_ms - g_samples.front().t_ms) > win)
  {
    g_samples.pop_front();
  }

  // 窓内増分を計算
  if (g_samples.size() >= 2)
    g_window_delta_mm = total_mm - g_samples.front().total_mm;
  else
    g_window_delta_mm = 0.0f;

  // 雨判定（①＋②）
  const float amount = rainfallStateSettings.amount_mm; // ①
  if (g_window_delta_mm >= amount)
  {
    g_state = RainState::RAINING;
    g_last_rain_ms = now_ms;
    return;
  }

  // 晴れ判定（③）
  if (g_state == RainState::RAINING)
  {
    const uint32_t dry = rainfallStateSettings.dry_time_sec * 1000UL; // ③
    if (g_last_rain_ms != 0 && (now_ms - g_last_rain_ms) >= dry)
    {
      g_state = RainState::DRY;
    }
  }
  else if (g_state == RainState::UNKNOWN)
  {
    // 追加パラメータ無しで UNKNOWN→DRY を落とす：
    // 「観測窓が一巡しても雨判定が出ない」なら DRY とみなす
    if (g_samples.size() >= 2 && (now_ms - g_samples.front().t_ms) >= win)
    {
      g_state = RainState::DRY;
    }
  }
}