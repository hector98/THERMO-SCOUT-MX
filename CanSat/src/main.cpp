#include <Wire.h>
#include <SSD1306Wire.h>
#include <LoRa.h>
#include "oled.h"
#include "lora.h"
#include "gps.h"
#include "bme.h"

SSD1306Wire display(0x3c, OLED_SDA, OLED_SCL);

//HardwareSerial SerialGPS(2);
TinyGPSPlus gps;

const int MQSensor = 36;

Adafruit_BME280 bme;

void setup() {
	Serial.begin(115200);
	pinMode(Vext, OUTPUT);
	digitalWrite(Vext, LOW);
	delay(100);

	// Inicializar OLED
	OLED_Init();
	PrintToOLED("Thermo Scout MX");
	PrintToOLED("CanSat");

	// Inicializar LoRa
	LoRa_Init();

	// Inicializar GPS
	GPS_Init();

	// Inicializar BME
	BME_Init();
	
}

void loop() {
	int rawValue = analogRead(MQSensor);       // Lectura de 0-4095
	float voltage = (rawValue * 3.3) / 4095.0;  // Convertir a voltaje (0-3.3V)

	//GPSRead();
	String data = "";
	data += "Valores Atmosfericos: " + BME_ReadData() + ". ";
	data += "Calidad del aire: " + String(rawValue) + ". ";
	data += "GPS: " + printGPSData();
												
	PrintToOLED("Enviando Datos: " + data);
	LoRa_Send(data);

}
