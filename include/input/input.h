// input/input.h
#pragma once

enum InputButton {
    BTN_NONE,
    BTN_UP,
    BTN_DOWN,
    BTN_RIGHT,
    BTN_LEFT,
    BTN_SELECT
};

enum InputEventType {
    INPUT_NONE,
    INPUT_PRESS,
    INPUT_RELEASE,
    INPUT_LONG_PRESS,
    INPUT_REPEAT
};

struct InputEvent {
    InputButton button;
    InputEventType type;
};

void initInput();

InputButton inputGetButton();
InputEvent inputGetEvent();

