#include <Wire.h>
#include <SSD1306Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include "oled.h"
#include <QMC5883LCompass.h>


//QMC5883LCompass compass;

#define SDA 4
#define SCL 15

void Magnetrometro_Init(void);
String getMagnetometro(void);
