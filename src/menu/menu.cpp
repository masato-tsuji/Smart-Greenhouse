#include "menu/menu.h"
#include "ui/display.h"
#include "input/input.h"


#include "actions/motor_control.h"  // motorSetMode, MODE_...
extern MenuItem manualMenu;         // ← menu_tree.cpp にある Manual Control

MenuItem* currentMenu = nullptr;
uint8_t cursor = 0;
bool actionRunning = false;

// 前回描画状態を記憶
static MenuItem* lastMenu = nullptr;
static uint8_t lastCursor = 255;

//  初期化
void initMenu() {
    currentMenu = &mainMenu;
    menuRender();        // 最初の画面表示
}

// Menu描画
void menuRender()
{
    static MenuItem* lastMenu = nullptr;
    static uint8_t lastCursor = 255;

    if (!currentMenu) return;

    bool menuChanged   = (lastMenu != currentMenu);
    bool cursorChanged = (lastCursor != cursor);

    // ⭐ ACTIONは常にdraw
    if (currentMenu->type == MENU_ACTION)
    {
        if (menuChanged) displayClear();
        if (currentMenu->draw) currentMenu->draw();

        lastMenu = currentMenu;
        lastCursor = cursor;
        return;
    }

    if (!menuChanged && !cursorChanged) return;

    if (menuChanged) displayClear();

    for (int i = 0; i < 2; i++)
    {
        int index = cursor + i;

        displaySetCursor(0, i);
        displayPrint("                ");

        if (index >= currentMenu->childCount) continue;

        displaySetCursor(0, i);
        displayPrint(i == 0 ? ">" : " ");
        displayPrint(currentMenu->children[index]->name);
    }

    lastMenu = currentMenu;
    lastCursor = cursor;
}


void menuUp() {
    if (actionRunning) return;
    if (cursor > 0) cursor--;
}

void menuDown() {
    if (actionRunning) return;
    if (cursor < currentMenu->childCount - 1) cursor++;
}

void menuEnter()
{
    MenuItem* item = currentMenu->children[cursor];
    if (!item) return;
    currentMenu->lastCursor = cursor;

    // 今の画面がACTIONなら白に戻す
    if (currentMenu->type == MENU_ACTION)
    {
        displaySetBacklight(DISP_WHITE);
    }

    currentMenu = item;
    cursor = currentMenu->lastCursor;

    // ★追加：Manual Control に入ったら手動モード
    if (currentMenu == &manualMenu)
    {
        motorSetMode(MODE_MANUAL);
    }

    // debug用
    // Serial.print("[menuEnter] now=");
    // Serial.print(currentMenu->name);
    // Serial.print(" type=");
    // Serial.println((int)currentMenu->type);

}

void menuBack()
{
    if (!currentMenu->parent) return;

    // 今がACTIONなら白に戻す
    if (currentMenu->type == MENU_ACTION)
    {
        displaySetBacklight(DISP_WHITE);
    }


    // ★追加：Manual Control から出るなら AUTOへ
    if (currentMenu == &manualMenu)
    {
        motorSetMode(MODE_AUTO);
    }

    MenuItem* parent = currentMenu->parent;
    cursor = parent->lastCursor;   // ← 親のカーソル復元
    currentMenu = parent;

}


static void handleMenuEvent(InputButton btn)
{
    switch (btn)
    {
        case BTN_UP:    menuUp(); break;
        case BTN_DOWN:  menuDown(); break;
        case BTN_RIGHT: menuEnter(); break;
        case BTN_LEFT:  menuBack(); break;
        default: break;
    }
}

// Menu更新
void menuUpdate()
{

    // debug用
    // static unsigned long lastDbg = 0;
    // unsigned long now = millis();
    // if (now - lastDbg >= 1000) {
    //     lastDbg = now;
    //     Serial.print("[menuUpdate] type=");
    //     Serial.print((int)(currentMenu ? currentMenu->type : -1));
    //     Serial.print(" refreshMs=");
    //     Serial.print((int)(currentMenu ? currentMenu->refreshMs : -1));
    //     Serial.print(" lastRefresh=");
    //     Serial.println((unsigned long)(currentMenu ? currentMenu->lastRefreshMs : 0));
    // }


    // 入力イベント（遷移/カーソル移動）
    InputEvent ev = inputGetEvent();
    if (ev.type == INPUT_PRESS || ev.type == INPUT_REPEAT)
    {
        handleMenuEvent(ev.button);
        menuRender();
        return;
    }

    // 入力が無いとき：ACTIONなら周期実行
    if (currentMenu && currentMenu->type == MENU_ACTION)
    {
        if (currentMenu->refreshMs == 0) return; // 自動更新なし

        unsigned long now = millis();
        if (now - currentMenu->lastRefreshMs >= currentMenu->refreshMs)
        {
            currentMenu->lastRefreshMs = now;

            // ★ここが重要：ACTIONの処理を回す
            if (currentMenu->action) currentMenu->action();

            // 表示（ACTIONはmenuRender内でdrawされる）
            menuRender();
        }
    }
}