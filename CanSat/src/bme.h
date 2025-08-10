#include <Adafruit_BME280.h>
#include <Wire.h>
#include <SSD1306Wire.h>
#include "oled.h"

#define SENSOR_ADDR 0x76

extern Adafruit_BME280 bme;

void BME_Init(void);
String BME_ReadData(void);
