// tools/cmd_status.cpp
#include "drivers/sensor.h"

struct SensorStatus
{
    float temperature;
    float humidity;
    bool valid;
};

namespace
{
    SensorStatus collectSensorStatus()
    {
        SensorStatus st{};
        st.temperature = readTemperature();
        st.humidity    = readHumidity();
        st.valid       = true;
        return st;
    }
}