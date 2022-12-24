#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <SPI.h>
#include <LSM6DSOXSensor.h>

LSM6DSOXSensor lsm6dsoxSensor(&SPI1, D6);

#define CONVERT_G_TO_MS2 9.80665f
#define FREQUENCY_HZ 100
#define INTERVAL_MS (1000 / (FREQUENCY_HZ))

volatile uint32_t last_interval_ms = 0;

void setup()
{
    Serial.begin(115200);

    SPI1.begin();

    lsm6dsoxSensor.begin();

    // Enable accelerometer and gyroscope, and check success
    if (lsm6dsoxSensor.Enable_X() != LSM6DSOX_OK || lsm6dsoxSensor.Enable_G() != LSM6DSOX_OK)
    {
        while (1)
            ;
    }

    // Read ID of device and check that it is correct
    uint8_t id;
    lsm6dsoxSensor.ReadID(&id);

    if (id != LSM6DSOX_ID)
    {
        while (1)
            ;
    }
    // Set accelerometer scale at +- 2G. Available values are +- 2, 4, 8, 16 G
    lsm6dsoxSensor.Set_X_FS(2);

    // Set gyroscope scale at +- 125 degres per second. Available values are +- 125, 250, 500, 1000, 2000 dps
    lsm6dsoxSensor.Set_G_FS(125);

    // Set Accelerometer sample rate to 208 Hz. Available values are +- 12.0, 26.0, 52.0, 104.0, 208.0, 416.0, 833.0, 1667.0, 3333.0, 6667.0 Hz
    lsm6dsoxSensor.Set_X_ODR(104.0f);

    // Set Gyroscope sample rate to 208 Hz. Available values are +- 12.0, 26.0, 52.0, 104.0, 208.0, 416.0, 833.0, 1667.0, 3333.0, 6667.0 Hz
    lsm6dsoxSensor.Set_G_ODR(104.0f);
}

void loop()
{

    if (millis() > last_interval_ms + INTERVAL_MS)
    {
        last_interval_ms = millis();
        // Read accelerometer
        uint8_t acceleroStatus;
        lsm6dsoxSensor.Get_X_DRDY_Status(&acceleroStatus);
        if (acceleroStatus == 1)
        { // Status == 1 means a new data is available

            int32_t acceleration[3];
            lsm6dsoxSensor.Get_X_Axes(acceleration);

            float x = (acceleration[0] / 1000.0) * CONVERT_G_TO_MS2;
            float y = (acceleration[1] / 1000.0) * CONVERT_G_TO_MS2;
            float z = (acceleration[2] / 1000.0) * CONVERT_G_TO_MS2;

            char buf[32] = {0};
            sprintf(buf, "%f,%f,%f\n", x, y, z);
            // Plot data for each axis in mg
            Serial.print(buf);
        }
    }
}