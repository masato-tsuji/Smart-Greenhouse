#include "drivers/rainfall.h"
#include <Arduino.h>
#include <Wire.h>

// DFRobot公式ライブラリ
#include "DFRobot_RainfallSensor.h"   // 公式サンプルがこれを使用 [2](https://wiki.dfrobot.com/SKU_SEN0575_Gravity_Rainfall_Sensor)

static DFRobot_RainfallSensor_I2C g_sensor(&Wire);  // I2Cモード [2](https://wiki.dfrobot.com/SKU_SEN0575_Gravity_Rainfall_Sensor)
static bool g_inited = false;
static bool g_ok = false;
static uint32_t g_last_ok_ms = 0;

void initRainfall()
{
  Wire.begin();

  // 任意：I2C速度（他デバイスと合わせて）
  // Wire.setClock(100000); // 100kHz
  // Wire.setClock(400000); // 400kHz

  // begin() が通信確認を行う（失敗する間はfalse） [2](https://wiki.dfrobot.com/SKU_SEN0575_Gravity_Rainfall_Sensor)[3](https://github.com/DFRobot/DFRobot_RainfallSensor)
  g_ok = g_sensor.begin();
  g_inited = true;
  if (g_ok) g_last_ok_ms = millis();
}

bool rainfallReadTotalMm(float &out_total_mm)
{
  if (!g_inited) initRainfall();

  // もし初回失敗していたら再トライ（500ms周期呼び出しなら重くならない）
  if (!g_ok)
  {
    g_ok = g_sensor.begin();
    if (!g_ok) return false;
  }

  // 累積降雨量(mm)を取得（公式API getRainfall） [2](https://wiki.dfrobot.com/SKU_SEN0575_Gravity_Rainfall_Sensor)[3](https://github.com/DFRobot/DFRobot_RainfallSensor)
  out_total_mm = g_sensor.getRainfall();
  g_last_ok_ms = millis();
  return true;
}

bool rainfallOk()
{
  return g_ok;
}

uint32_t rainfallLastOkMs()
{
  return g_last_ok_ms;
}