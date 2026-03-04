// input_lcdshield.cpp

#include <Arduino.h>
#include "input/input.h"
#include "drivers/lcdshield.h"

#define LONG_PRESS_TIME   500 // 長押し判定（ms）
#define REPEAT_INTERVAL   200 // 長押しリピート間隔（ms）

void initInput(){}  // 現時点では未使用

InputButton inputGetButton()
{
    uint8_t buttons = lcd.readButtons();
    InputButton current = BTN_NONE;

    if (buttons & BUTTON_UP)    current = BTN_UP;
    if (buttons & BUTTON_DOWN)  current = BTN_DOWN;
    if (buttons & BUTTON_RIGHT) current = BTN_RIGHT;
    if (buttons & BUTTON_LEFT)  current = BTN_LEFT;

    return current;
}

InputEvent inputGetEvent()
{
    static InputButton lastButton = BTN_NONE;
    static unsigned long pressStart = 0;
    static unsigned long lastRepeat = 0;

    InputEvent ev = {BTN_NONE, INPUT_NONE};
    InputButton current = inputGetButton();

    unsigned long now = millis();

    // 押した瞬間
    if (current != BTN_NONE && lastButton == BTN_NONE)
    {
        pressStart = now;
        lastRepeat = now;
        ev = {current, INPUT_PRESS};
    }

    // 押しっぱなし
    else if (current != BTN_NONE && lastButton == current)
    {
        if (now - pressStart > LONG_PRESS_TIME)  // 長押し開始
        {
            if (now - lastRepeat > REPEAT_INTERVAL)
            {
                lastRepeat = now;
                ev = {current, INPUT_REPEAT};
            }
        }
    }

    // 離した
    else if (current == BTN_NONE && lastButton != BTN_NONE)
    {
        ev = {lastButton, INPUT_RELEASE};
    }

    lastButton = current;
    return ev;
}

