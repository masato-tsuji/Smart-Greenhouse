// menu/menu.h
#pragma once
#include <Arduino.h>
#include <Adafruit_RGBLCDShield.h>

// ===== 種類 =====
enum MenuType
{
    MENU_FOLDER,
    MENU_ACTION,
    MENU_VALUE
};

// ===== 構造体 =====
struct MenuItem
{
    const char* name;
    MenuType type;

    MenuItem* parent;
    MenuItem** children;
    uint8_t childCount;

    void (*action)();
    void (*draw)();

    int* value;
    int minValue;
    int maxValue;

    uint8_t lastCursor;

    // 更新周期
    uint16_t refreshMs;          // 例: 5000 = 5秒ごと 0は更新しない
    unsigned long lastRefreshMs; // 内部用（前回更新時刻）
};


// ===== ルート =====
extern MenuItem mainMenu;

// ===== 状態 =====
// extern MenuItem* currentMenu;
// extern uint8_t cursor;
// extern bool actionRunning;

extern MenuItem* currentMenu;
extern uint8_t cursor;
extern bool actionRunning;
extern Adafruit_RGBLCDShield lcd;


// ===== 操作API =====
void initMenu();
void menuUpdate();
void menuRender();
void menuUp();
void menuDown();
void menuEnter();
void menuBack();

