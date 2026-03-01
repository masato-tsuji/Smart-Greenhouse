
#include "input/input.h"


// drivers/motor.h
#pragma once
#include <Arduino.h>

enum MotorCommand : uint8_t {
  MOTOR_CMD_STOP = 0,
  MOTOR_CMD_OPEN,   // 上昇/正転
  MOTOR_CMD_CLOSE   // 下降/逆転
};

void initMotor();
void motorStop();

// 追加：共通コマンド（LS安全込み）
void motorCommand(MotorCommand cmd);

// 任意：状態参照
uint8_t motorGetDir(); // 0 stop, 1 up, 2 down

// 手動操作（BTN_UP/BTN_DOWNを渡す：押してる間だけ動く）
void motorManualUpdate(InputButton btn);