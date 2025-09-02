#include "gps.h"

HardwareSerial SerialGPS(2);

void GPS_Init() {
	Serial.begin(115200);
	SerialGPS.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX); 

	//PrintToOLED("GPS Inicializado");
}

String getGPSLocation(void){
	String data = "";

	if (SerialGPS.available() > 0) {	
		if (gps.encode(SerialGPS.read())) {
			if (gps.location.isValid()) {
				data += "GPS Lat: " + String(gps.location.lat(), 6);
				data += "~";
				data += "GPS Lon: " + String(gps.location.lng(), 6);
				data += "~";
			}
		}
	}
}

