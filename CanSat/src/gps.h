#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "oled.h"

#define GPS_RX 16
#define GPS_TX 17

extern TinyGPSPlus gps;

void GPS_Init(void);
void GPSRead(void);
//void printGPSData(void);
