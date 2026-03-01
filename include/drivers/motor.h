#pragma once
#include <Arduino.h>
#include "input/input.h"

// モーター指令（上昇/下降/停止）
enum MotorCommand : uint8_t {
    MOTOR_CMD_STOP = 0,
    MOTOR_CMD_OPEN,   // 上昇（正転）
    MOTOR_CMD_CLOSE   // 下降（逆転）
};

void initMotor();
void motorStop();
uint8_t motorGetDir();                // 0:stop 1:up 2:down
void motorCommand(MotorCommand cmd);  // 安全・上限・ランプ込み

// 互換（手動制御。将来actionsへ移動しやすい）
void motorManualUpdate(InputButton btn);