#include "bme.h"


void BME_Init(void){
    Wire.begin(4, 15);  // SDA=GPIO4, SCL=GPIO15

	// Verificar inicialización
    bme.begin(SENSOR_ADDR);

	//PrintToOLED("BME280 Inicializado");
}

float getAltitude(void){
	float altitude = (44330 * (1.0 - pow(bme.readPressure() / 100.0F / 1013.25, 0.1903)));
	return altitude;
}

String getTemperature(void){
	String temperature = "Temperatura: " + String(bme.readTemperature()); // Temperatura en °C
	return temperature;
}

String getPressure(void){
	String pressure = "Presión: " + String(bme.readPressure() / 100.0F); // Presion en hPa
	return pressure;
}

String getHumidity(void){
	String humidity = "Humedad: " + String(bme.readHumidity()); // Humedad en %
	return humidity;
}
