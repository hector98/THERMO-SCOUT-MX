#include "bme.h"


void BME_Init(void){
    Wire.begin(4, 15);  // SDA=GPIO4, SCL=GPIO15

	// Verificar inicialización
    bme.begin(SENSOR_ADDR);

	PrintToOLED("BME280 Inicializado");
}

String BME_ReadData(void){
	String data = "";

	data += "Temperatura: " + String(bme.readTemperature()) + " °C. ";
	data += "Presión: " + String(bme.readPressure() / 100.0F) + " hPa. ";
	data += "Humedad: " + String(bme.readHumidity()) + " %.";
	
	return data;
}
