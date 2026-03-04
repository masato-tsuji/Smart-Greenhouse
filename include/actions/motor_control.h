#pragma once
#include <Arduino.h>

// モーター制御モード
enum MotorControlMode : uint8_t
{
    MODE_STOP = 0,
    MODE_MANUAL,
    MODE_AUTO
};

// モード設定/取得
void motorSetMode(MotorControlMode mode);
MotorControlMode motorGetMode();

// 周期更新（menuのrefreshMsなどから呼ぶ）
void motorControlUpdate(uint32_t now_ms);

struct MotorStatus
{
    uint8_t dir;        // 0 stop, 1 up, 2 down
    // 以下実装中
    // uint8_t pwm;        // 現在出力
    // bool upperLimit;
    // bool lowerLimit;
};

MotorStatus motorGetStatus();