#include "gps.h"

HardwareSerial SerialGPS(1);

float latitude = 0.0;
float longitude = 0.0;
int satelites = 0;
String timeStr = "";
String dateStr = "";
bool validFix = false;

void GPS_Init() {
	SerialGPS.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX); 

	//PrintToOLED("GPS Inicializado");
}

int splitString(String data, char delimiter, String* output, int maxCount) {
  int count = 0;
  int startIndex = 0;
  int endIndex = 0;
  
  while (endIndex < data.length() && count < maxCount) {
    endIndex = data.indexOf(delimiter, startIndex);
    
    if (endIndex == -1) {
      output[count++] = data.substring(startIndex);
      break;
    }
    
    output[count++] = data.substring(startIndex, endIndex);
    startIndex = endIndex + 1;
  }
  
  return count;
}

float convertToDegrees(String value) {
  // Formato DDMM.MMMMM -> DD + MM.MMMMM/60
  if (value.length() < 4) return 0.0;
  
  int dotPos = value.indexOf('.');
  if (dotPos == -1) return 0.0;
  
  String degStr = value.substring(0, dotPos - 2);
  String minStr = value.substring(dotPos - 2);
  
  float degrees = degStr.toFloat();
  float minutes = minStr.toFloat();
  
  return degrees + (minutes / 60.0);
}

void processGGA(String data) {
  // Ejemplo: $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
  String parts[15];
  int count = splitString(data, ',', parts, 15);
  
  if (count < 10) return;
  
  // Tiempo UTC
  if (parts[1].length() >= 6) {
    timeStr = parts[1].substring(0, 2) + ":" + 
              parts[1].substring(2, 4) + ":" + 
              parts[1].substring(4, 6);
  }
  
  // Fix quality (1=GPS fix, 2=DGPS fix)
  validFix = (parts[6].toInt() > 0);
  
  // Número de satélites
  satelites = parts[7].toInt();
  
  // Latitud
  if (parts[2].length() > 0 && parts[3].length() > 0) {
    latitude = convertToDegrees(parts[2]);
    if (parts[3] == "S") latitude = -latitude;
  }
  
  // Longitud
  if (parts[4].length() > 0 && parts[5].length() > 0) {
    longitude = convertToDegrees(parts[4]);
    if (parts[5] == "W") longitude = -longitude;
  }
}

void processRMC(String data) {
  // Ejemplo: $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
  String parts[15];
  int count = splitString(data, ',', parts, 15);
  
  if (count < 10) return;
  
  // Fecha (DDMMYY)
  if (parts[9].length() == 6) {
    dateStr = parts[9].substring(0, 2) + "/" + 
              parts[9].substring(2, 4) + "/" + 
              "20" + parts[9].substring(4, 6);
  }
}


String printGPSData() {
	String data = "--- Datos GPS ---";

	if (satelites != 0) {
		data += "Satellites: " + String(satelites) + ". ";
		data += "Time: " + timeStr + ". ";
		data += "Date: " + dateStr + ". ";
		data += "Location: ";
		data += "Longitude: " + String(longitude, 6) + ". ";
		data += "Latitude: " + String(latitude, 6) + ". ";
	} else {
		data += "Sin Satelites disponibles";
	}

	return data;
}

void GPSRead() {
	if (SerialGPS.available()) {
		String nmea = SerialGPS.readStringUntil('\n');
		PrintToOLED(nmea);
		Serial.println(nmea);

		if (nmea.startsWith("$GPGGA")) {
			processGGA(nmea);
		} else if (nmea.startsWith("$GPRMC")) {
			processRMC(nmea);
		}

		// Mostrar datos cada 2 segundos
		static unsigned long lastPrint = 0;
		if (millis() - lastPrint > 2000) {
			printGPSData();
			lastPrint = millis();
		}
	}
}


