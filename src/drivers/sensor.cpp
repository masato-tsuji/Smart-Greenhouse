#include "drivers/sensor.h"
#include <arduino-sht.h>
#include <SHTSensor.h>

SHTSensor sht;  

//  初期化
void initSensor() {
    sht.init();  // setup() で一度だけ呼ぶ
}

float readTemperature() {
    sht.init();
    if (sht.readSample()) return sht.getTemperature();
    return -999.0;  // エラー値
}

float readHumidity() {
    if (sht.readSample()) return sht.getHumidity();
    return -1.0;    // エラー値
}


