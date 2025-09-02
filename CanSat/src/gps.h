#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "oled.h"

#define GPS_RX 3
#define GPS_TX 1

extern TinyGPSPlus gps;

void GPS_Init(void);
String getGPSLocation(void);
void GPSRead(void);
void GPS_Data(void);
String printGPSData(void);
