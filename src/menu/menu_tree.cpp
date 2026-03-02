#include "menu/menu.h"
#include "menu/menu_actions.h"
#include "config/settings.h"

#define COUNT_OF(x) (sizeof(x) / sizeof((x)[0]))

//==================================================
// 先行宣言（子配列で参照するため）
//==================================================
extern MenuItem mainMenu;

extern MenuItem tempHumiMenu;
extern MenuItem controlMenu;
extern MenuItem manualMenu;
extern MenuItem sensorMenu;
extern MenuItem logsMenu;
extern MenuItem timeMenu;
extern MenuItem toolsMenu;
extern MenuItem systemMenu;

extern MenuItem openTempMenu;
extern MenuItem closeTempMenu;

extern MenuItem motorManualMenu;
// extern MenuItem motorOpenMenu;
// extern MenuItem motorCloseMenu;

//==================================================
// 子配列（UI順を優先しつつ、コメントで関係を明確化）
//==================================================
MenuItem* mainChildren[] = {
    // --- Monitor（普段見る）
    &tempHumiMenu,

    // --- Setup / Manual（操作系）
    &controlMenu,
    &manualMenu,

    // --- Status / Logs（状態・履歴）
    &sensorMenu,
    &logsMenu,

    // --- Time / Tools / System（保守）
    &timeMenu,
    &toolsMenu,
    &systemMenu
};

MenuItem* controlChildren[] = {
    &openTempMenu,
    &closeTempMenu
};

MenuItem* manualChildren[] = {
    &motorManualMenu
    // &motorOpenMenu,
    // &motorCloseMenu
};

//==================================================
// ルート
//==================================================
MenuItem mainMenu = {
    "Main",
    MENU_FOLDER,
    nullptr,
    mainChildren,
    COUNT_OF(mainChildren),
    nullptr,
    nullptr,
    nullptr, 0, 0,
    0,
    0, 0 // refreshMs, lastRefreshMs（FOLDERは未使用）
};

//==================================================
// Monitor（温湿度）
//==================================================
MenuItem tempHumiMenu = {
    "Temp/Humi",
    MENU_ACTION,
    &mainMenu,
    nullptr, 0,
    actionShowTempHumidity,
    drawTempHumidity,
    nullptr, 0, 0,
    0,
    5000, 0  // refreshMs=5秒（自動更新）
};

//==================================================
// Setup（制御設定）
//==================================================
MenuItem controlMenu = {
    "Control Setup",
    MENU_FOLDER,
    &mainMenu,
    controlChildren,
    COUNT_OF(controlChildren),
    nullptr,
    nullptr,
    nullptr, 0, 0,
    0,
    0, 0
};

MenuItem openTempMenu = {
    "Open Temp",
    MENU_VALUE,
    &controlMenu,
    nullptr, 0,
    nullptr,
    nullptr,
    &tempSettings.openTemp,
    10, 40,
    0,
    0, 0
};

MenuItem closeTempMenu = {
    "Close Temp",
    MENU_VALUE,
    &controlMenu,
    nullptr, 0,
    nullptr,
    nullptr,
    &tempSettings.closeTemp,
    10, 40,
    0,
    0, 0
};

MenuItem manualMenu = {
    "Manual Control",
    MENU_ACTION,
    &mainMenu,
    nullptr, 0,
    actionMotorManual,
    drawMotorManual,
    nullptr, 0, 0,
    0,
    20, 0
};

MenuItem motorManualMenu = {
    "UP/DW LONG PRESS",
    MENU_ACTION,
    &manualMenu,
    nullptr, 0,
    actionMotorManual,
    drawMotorManual,
    nullptr, 0, 0,
    0,
    20, 0   // ここは将来、20msとかにして“画面＆制御”を定期更新してもOK
};


MenuItem motorOpenMenu = {
    "Motor Open",
    MENU_ACTION,
    &manualMenu,
    nullptr,0,
    actionMotorOpen,
    drawMotorOpen,
    nullptr,0,0,
    0,
    0, 0
};

MenuItem motorCloseMenu = {
    "Motor Close",
    MENU_ACTION,
    &manualMenu,
    nullptr,0,
    actionMotorClose,
    drawMotorClose,
    nullptr,0,0,
    0,
    0, 0
};


//==================================================
// Status / Logs（状態・ログ）
//==================================================
MenuItem sensorMenu = {
    "Sensor Status",
    MENU_ACTION,
    &mainMenu,
    nullptr, 0,
    actionSensorStatus,
    drawSensorStatus,
    nullptr, 0, 0,
    0,
    100, 0
};

MenuItem logsMenu = {
    "View Logs",
    MENU_ACTION,
    &mainMenu,
    nullptr, 0,
    actionViewLogs,
    drawLogs,
    nullptr, 0, 0,
    0,
    0, 0
};

//==================================================
// Maintenance（時刻・ツール・システム）
//==================================================
MenuItem timeMenu = {
    "Time Setup",
    MENU_ACTION,
    &mainMenu,
    nullptr, 0,
    actionTimeSetup,
    drawTimeSetup,
    nullptr, 0, 0,
    0,
    0, 0
};

MenuItem toolsMenu = {
    "Tools",
    MENU_FOLDER,
    &mainMenu,
    nullptr, 0,
    nullptr,
    nullptr,
    nullptr, 0, 0,
    0,
    0, 0
};

MenuItem systemMenu = {
    "System Info",
    MENU_ACTION,
    &mainMenu,
    nullptr, 0,
    actionSystemInfo,
    drawSystemInfo,
    nullptr, 0, 0,
    0,
    0, 0
};