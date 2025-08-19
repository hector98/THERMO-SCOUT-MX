#include <Wire.h>
#include <SSD1306Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

//extern Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(); // ID único

void magInit(void);
String magReadData(void);
