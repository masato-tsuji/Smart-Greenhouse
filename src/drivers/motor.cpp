#include "drivers/motor.h"

//==============================
// SHIELD-MD10 の制御ピン
//  ※ジャンパでこのピンに合わせてください
//    PWM: D6 / DIR: D7
//==============================
static constexpr uint8_t PIN_MOTOR_PWM = 3; // PWM input
static constexpr uint8_t PIN_MOTOR_DIR = 2; // DIR input

//==============================
// リミットスイッチ（B接点=NC）
//  D4: 上昇側LS（離れたら停止）
//  D5: 下降側LS（離れたら停止）
//==============================
static constexpr uint8_t PIN_LS_UP   = 4; // D4
static constexpr uint8_t PIN_LS_DOWN = 5; // D5

// B接点（NC）を INPUT_PULLUP で読む前提：
//  通常(未作動)=導通→LOW
//  作動(離れる)=開放→HIGH  →「離れたら停止」

// 本番用 B接点
// static inline bool lsUpReleased()   { return digitalRead(PIN_LS_UP)   == HIGH; }
// static inline bool lsDownReleased() { return digitalRead(PIN_LS_DOWN) == HIGH; }

// debug用　A接点
static inline bool lsUpReleased()   { return digitalRead(PIN_LS_UP)   == LOW; }
static inline bool lsDownReleased() { return digitalRead(PIN_LS_DOWN) == LOW; }


//==============================
// 内部状態
//==============================
enum MotorDir : uint8_t { DIR_STOP=0, DIR_UP=1, DIR_DOWN=2 };
static MotorDir g_dir = DIR_STOP;

// 速度（必要なら後で可変に）
static constexpr uint8_t MOTOR_SPEED = 255;

//------------------------------
// 低レベル駆動（SHIELD-MD10は PWM+DIR）
//------------------------------
static void driveUp()
{
  // 正転（上昇）: DIR=HIGH, PWM=速度
  digitalWrite(PIN_MOTOR_DIR, HIGH);
  analogWrite(PIN_MOTOR_PWM, MOTOR_SPEED);
  g_dir = DIR_UP;
}

static void driveDown()
{
  // 逆転（下降）: DIR=LOW, PWM=速度
  digitalWrite(PIN_MOTOR_DIR, LOW);
  analogWrite(PIN_MOTOR_PWM, MOTOR_SPEED);
  g_dir = DIR_DOWN;
}

void motorStop()
{
  // 停止：PWM=0（基本これで止まります）
  analogWrite(PIN_MOTOR_PWM, 0);
  g_dir = DIR_STOP;
}

//------------------------------
// 初期化
//------------------------------
void initMotor()
{
  pinMode(PIN_MOTOR_PWM, OUTPUT);
  pinMode(PIN_MOTOR_DIR, OUTPUT);

  pinMode(PIN_LS_UP,   INPUT_PULLUP);
  pinMode(PIN_LS_DOWN, INPUT_PULLUP);

  motorStop();
}

//------------------------------
// 手動操作：押している間だけ動く
// ・UP押下中：正転。ただし D4(上LS)が「離れた」ら即停止
// ・DOWN押下中：逆転。ただし D5(下LS)が「離れた」ら即停止
// ・どちらも押してない：停止
//------------------------------
void motorManualUpdate(InputButton btn)
{
  // まず安全：動作中にLSが離れたら即停止
  if (g_dir == DIR_UP && lsUpReleased())
  {
    motorStop();
    return;
  }
  if (g_dir == DIR_DOWN && lsDownReleased())
  {
    motorStop();
    return;
  }

  // 押している間だけ動く
  if (btn == BTN_UP)
  {
    // すでに上限なら動かさない
    if (lsUpReleased())
    {
      motorStop();
      return;
    }

    if (g_dir != DIR_UP)
    {
      motorStop(); // 念のため切替前に停止
      driveUp();
    }
    return;
  }

  if (btn == BTN_DOWN)
  {
    // すでに下限なら動かさない
    if (lsDownReleased())
    {
      motorStop();
      return;
    }

    if (g_dir != DIR_DOWN)
    {
      motorStop();
      driveDown();
    }
    return;
  }

  // 押してない
  motorStop();
}