# Smart-Greenhouse

Arduino UNO R4 WiFi を用いて、ビニールハウスの **巻き上げモーター** を制御するためのファームウェアです。  
LCD + ボタンによるローカル操作を基本とし、雨量・環境センサーの状態に応じた制御を行います。

> **Status: WIP（開発中）**  
> 現在は「Arduino 単体で安全に制御できること」を最優先で開発しています。  
> 通信（LoRa 等）は将来拡張として検討中です。

---

## 目的 / ゴール
- 巻き上げモーターを **安全に「開 / 閉 / 停止」** できる
- LCD で状態確認・操作ができる
- 雨量などの外部状態を制御判断に使える
- 組込みとして **周期が崩れない・危険側に倒れない** 設計にする

---

## システム構成概要
本システムは Arduino UNO R4 WiFi を制御コアとし、
温度・位置などの入力に基づいてビニールハウスの
巻き上げモーターを開閉する制御システムである。

Arduino は以下の役割を担う。

- センサー入力の取得（温度・雨量・位置）
- 制御判断（どこまで開閉するか）
- モーター駆動指令の生成
- 状態の可視化（LCD）
- 時刻管理・ログ（RTC / データロガー）


### 制御コア構成
- Arduino UNO R4 WiFi  
  - 制御ロジックの実行主体
  - 各種インターフェース（GPIO / I2C / SPI）の起点

- RGB LCD Shield + MCP23017 Shield
  - 状態表示および操作入力
  - I2C バス接続による I/O 拡張

- Motor Driver Shield

- Data Logger/RTC Shield

---
## 外部機器・インターフェース仕様
本システムで Arduino に接続される外部機器と、
それぞれのインターフェースおよびアドレス/ピン割り当てを示す。

機器名           | 型式 / 備考          | I/F  | Arduino入力 | Arduino出力 | I2C Addr / Pin
-----------------|----------------------|------|-------------|-------------|---------------
RGB LCD          | ADA-716              | I2C  | -           | 表示制御    | 0x20
温湿度センサー   | GROVE - SHT41搭載      | I2C  | 温湿度値    | -           | 0x44
降雨量センサー   | DFRobot              | I2C  | 降雨量      | -           | 0x1D
RTC              | DS3231想定           | I2C  | 時刻情報    | -           | 0x68
データロガー     | SDカード             | SPI  | -           | ログ書込    | CS/CLK/MOSI
モータードライバ | リレー/SSR等         | GPIO | -           | DIR         | D2
                 |                      | GPIO | -           | PWM         | D3
上昇端LS         | 上限位置検出         | GPIO | ON/OFF      | -           | D4
下降端LS         | 下限位置検出         | GPIO | ON/OFF      | -           | D5


---

## 開発環境
- VS Code + PlatformIO
- Framework: Arduino
- PlatformIO env: `uno_r4_wifi`

### platformio.ini（要点）
- platform: `renesas-ra`
- board: `uno_r4_wifi`
- monitor_speed: 115200
- monitor_filters: `time`
- monitor_echo: yes

### 依存ライブラリ
- adafruit/RTClib@2.1.4
- adafruit/Adafruit RGB LCD Shield Library@1.2.2
- adafruit/Adafruit MCP23017 Arduino Library@2.3.2
- DFRobot/DFRobot_RainfallSensor@1.0.0
- DFRobot/DFRobot_RTU@1.0.6
- sensirion/arduino-sht@1.2.6

---

## ビルド / 書き込み / モニタ

### ビルド
```bash
pio run
```

### 書き込み
```bash
pio run -t upload
```

### モニター
```bash
pio monitor
```