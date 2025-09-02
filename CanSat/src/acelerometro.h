#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define SDA 4
#define SCL 15

void Acelerometro_Init(void);
String getAcelerometro(void);
