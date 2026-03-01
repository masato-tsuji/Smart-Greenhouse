#include "drivers/motor.h"
#include "config/settings.h"  // ← settings.cppで反映した設定を読む
#include "input/input.h"      // motorManualUpdateを残す場合だけ

//==============================
// SHIELD-MD10 pins（あなたの実機設定）
//==============================
static constexpr uint8_t PIN_MOTOR_DIR = 2;
static constexpr uint8_t PIN_MOTOR_PWM = 3;

// 上下端リミットスイッチ
static constexpr uint8_t PIN_LS_UP   = 4;
static constexpr uint8_t PIN_LS_DOWN = 5;

//==============================
// LS論理（ここだけで切替）
// B接点(NC)+INPUT_PULLUP なら：通常LOW / 作動でHIGH
// A接点(NO)+INPUT_PULLUP なら：通常HIGH / 作動でLOW
//==============================
#define LS_TRIG_IS_LOW 0  // LS検知が(A接点)なら1、B接点(NC)なら0にする

static inline bool lsUpTriggered()
{
#if LS_TRIG_IS_LOW
  return digitalRead(PIN_LS_UP) == LOW;
#else
  return digitalRead(PIN_LS_UP) == HIGH;
#endif
}

static inline bool lsDownTriggered()
{
#if LS_TRIG_IS_LOW
  return digitalRead(PIN_LS_DOWN) == LOW;
#else
  return digitalRead(PIN_LS_DOWN) == HIGH;
#endif
}

//==============================
// 状態
//==============================
enum MotorDir : uint8_t { DIR_STOP=0, DIR_UP=1, DIR_DOWN=2 };
static MotorDir g_dir = DIR_STOP;

//==============================
// 上限＆ランプ（時間指定）
// - 上限: 0..100%（100超は100にクランプ）
// - ランプ: rampTimeMs で 0→目標PWM に線形上昇
//==============================
static uint8_t  g_targetPwm = 0;           // 目標PWM(0..255)
static uint8_t  g_currentPwm = 0;          // 現在PWM(0..255)
static unsigned long g_rampStartMs = 0;    // ランプ開始時刻

static inline uint8_t clampPct(uint8_t pct)
{
    return (pct > 100) ? 100 : pct;
}

static inline uint8_t pctToPwm(uint8_t pct)
{
    pct = clampPct(pct);
    return (uint16_t)pct * 255 / 100;
}

// 方向はここでセット（PWMはランプ側で制御）
static inline void setDirUp()
{
    digitalWrite(PIN_MOTOR_DIR, HIGH);
    g_dir = DIR_UP;
}

static inline void setDirDown()
{
    digitalWrite(PIN_MOTOR_DIR, LOW);
    g_dir = DIR_DOWN;
}

// ランプ開始（方向切替や停止→駆動開始時に呼ぶ）
static inline void startRamp(uint8_t targetPwm)
{
    g_targetPwm = targetPwm;
    g_currentPwm = 0;                 // 0から上げる（要件通り）
    g_rampStartMs = millis();
    analogWrite(PIN_MOTOR_PWM, 0);
}

// ランプ更新（周期的に motorCommand() が呼ばれる前提）
static inline void updateRamp(uint16_t rampTimeMs)
{
    // 目標が0なら止める（※停止は安全優先で即時停止）
    if (g_targetPwm == 0)
    {
        if (g_currentPwm != 0)
        {
            g_currentPwm = 0;
            analogWrite(PIN_MOTOR_PWM, 0);
        }
        return;
    }

    // ランプ時間=0なら即時
    if (rampTimeMs == 0)
    {
        if (g_currentPwm != g_targetPwm)
        {
            g_currentPwm = g_targetPwm;
            analogWrite(PIN_MOTOR_PWM, g_currentPwm);
        }
        return;
    }

    unsigned long elapsed = millis() - g_rampStartMs;

    if (elapsed >= rampTimeMs)
    {
        if (g_currentPwm != g_targetPwm)
        {
            g_currentPwm = g_targetPwm;
            analogWrite(PIN_MOTOR_PWM, g_currentPwm);
        }
        return;
    }

    // 線形上昇：target * (elapsed/rampTime)
    uint32_t pwm = (uint32_t)g_targetPwm * (uint32_t)elapsed / (uint32_t)rampTimeMs;
    uint8_t next = (uint8_t)pwm;

    // 変化があるときだけ書く（無駄なanalogWrite削減）
    if (next != g_currentPwm)
    {
        g_currentPwm = next;
        analogWrite(PIN_MOTOR_PWM, g_currentPwm);
    }
}

//------------------------------
// 停止（安全優先：即時停止）
//------------------------------
void motorStop()
{
    g_targetPwm = 0;
    g_currentPwm = 0;
    analogWrite(PIN_MOTOR_PWM, 0);
    g_dir = DIR_STOP;
}

void initMotor()
{
    pinMode(PIN_MOTOR_PWM, OUTPUT);
    pinMode(PIN_MOTOR_DIR, OUTPUT);

    pinMode(PIN_LS_UP,   INPUT_PULLUP);
    pinMode(PIN_LS_DOWN, INPUT_PULLUP);

    motorStop();
}

uint8_t motorGetDir()
{
    return (uint8_t)g_dir;
}

//------------------------------
// 反転（安全のため一旦停止→待機）
//------------------------------
static unsigned long lastStopTime = 0;
static const unsigned long REVERSE_DELAY_MS = 100;

//------------------------------
// 共通コマンド（安全＋上限＋ランプ込み）
//------------------------------
void motorCommand(MotorCommand cmd)
{
    // settings から取得（あなたが settings.cpp に反映済みの想定）
    // 例：motorSettings.limitPercent, motorSettings.rampTimeMs
    const uint8_t  limitPct   = clampPct(motorSettings.limitPercent);
    const uint16_t rampTimeMs = motorSettings.rampTimeMs;

    const uint8_t maxPwm = pctToPwm(limitPct);

    // 1) LS到達時は即停止（安全最優先）
    if (g_dir == DIR_UP && lsUpTriggered())    { motorStop(); }
    if (g_dir == DIR_DOWN && lsDownTriggered()) { motorStop(); }

    // STOP指令は即停止
    if (cmd == MOTOR_CMD_STOP)
    {
        motorStop();
        return;
    }

    // 2) 方向が変わる場合：一旦停止して待機
    if ((cmd == MOTOR_CMD_OPEN  && g_dir == DIR_DOWN) ||
        (cmd == MOTOR_CMD_CLOSE && g_dir == DIR_UP))
    {
        motorStop();
        lastStopTime = millis();
        return; // 次周期で同じcmdが来れば、その後に動く
    }

    // 3) 反転後の待ち時間
    if (g_dir == DIR_STOP && (millis() - lastStopTime < REVERSE_DELAY_MS))
    {
        return;
    }

    // 4) 指令に応じて方向設定＋ランプ開始/継続
    switch (cmd)
    {
        case MOTOR_CMD_OPEN:
        {
            // 上限なら動かさない
            if (lsUpTriggered()) { motorStop(); return; }

            // 停止中→上昇開始 または 下降→反転（上で処理済み）
            if (g_dir != DIR_UP)
            {
                // 念のため停止（DIRの変更前にPWMが出ないように）
                motorStop();
                setDirUp();
                startRamp(maxPwm);
            }
            else
            {
                // 上昇継続：目標PWMだけ維持（設定変更にも追随）
                g_targetPwm = maxPwm;
            }

            updateRamp(rampTimeMs);
            break;
        }

        case MOTOR_CMD_CLOSE:
        {
            // 下限なら動かさない
            if (lsDownTriggered()) { motorStop(); return; }

            if (g_dir != DIR_DOWN)
            {
                motorStop();
                setDirDown();
                startRamp(maxPwm);
            }
            else
            {
                g_targetPwm = maxPwm;
            }

            updateRamp(rampTimeMs);
            break;
        }

        default:
            motorStop();
            break;
    }
}

//------------------------------
// 互換：手動（後でmanual_controlに移動しやすい）
//------------------------------
void motorManualUpdate(InputButton btn)
{
    if (btn == BTN_UP)   { motorCommand(MOTOR_CMD_OPEN);  return; }
    if (btn == BTN_DOWN) { motorCommand(MOTOR_CMD_CLOSE); return; }
    motorCommand(MOTOR_CMD_STOP);
}