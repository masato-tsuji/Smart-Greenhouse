// actions/temp_humi.cpp

#include "actions/temp_humi.h"
#include "drivers/sensor.h"
#include "menu/menu.h"
#include "ui/display.h"

void actionShowTempHumidity() {

}

// 温湿度を表示
void drawTempHumidity()
{
    // 温湿度取得
    float t = readTemperature();
    float h = readHumidity();

    // ----------------------------
    // 温度によってバックライト色変更
    // ----------------------------
    if (t >= 30.0)
    {
        displaySetBacklight(DISP_RED);
    }
    else if (t >= 18.0)
    {
        displaySetBacklight(DISP_GREEN);
    }
    else
    {
        displaySetBacklight(DISP_BLUE);
    }

    // ----------------------------
    // 表示
    // ----------------------------
    displaySetCursor(0,0);
    displayPrint("Temp:");
    displayPrint(t,1);
    displayWrite(223);  // 記号char codeを1Byteで出力するためWrite
    displayPrint("C   ");

    displaySetCursor(0,1);
    displayPrint("Humi:");
    displayPrint(h,1);
    displayPrint("%   ");
}

