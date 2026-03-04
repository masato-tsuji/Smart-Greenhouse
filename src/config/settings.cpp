// settings.cpp
// 外部から設定する項目をここにまとめる
#include "config/settings.h"


TempSettings tempSettings =
{
    28,   // open temp
    15    // close temp
};


MotorSettings motorSettings =
{
    100,   // 初期上限
    0      // 初期ランプ時間
};


RainfallStateSettings rainfallStateSettings = {
  3,      // 3mm
  300,    // 5分
  180     // 3分
};