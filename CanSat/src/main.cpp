#include <Wire.h>
#include <SSD1306Wire.h>
#include <LoRa.h>
#include "oled.h"
#include "lora.h"
#include "gps.h"
#include "bme.h"
#include "magnetrometro.h"
#include "mq_sensor.h"

//Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345); // ID único
SSD1306Wire display(0x3c, OLED_SDA, OLED_SCL);

TinyGPSPlus gps;

Adafruit_BME280 bme;

void setup() {
	Serial.begin(115200);
	pinMode(Vext, OUTPUT);
	digitalWrite(Vext, LOW);
	delay(100);

	// Inicializar OLED
	OLED_Init();
	//PrintToOLED("Thermo Scout MX");
	//PrintToOLED("CanSat");
	PrintTwoLines("ING Electronica", "Thermo Scout MX", "CanSat");

	LoRa_Init(); // Inicializar LoRa

	GPS_Init(); // Inicializar GPS

	BME_Init(); // Inicializar BME
			
	PrintTwoLines("BME280 OK", "LoRa OK", "GPS OK");

}

int i = 0;
float alt_init = 0;
float alt_base = 0;

void loop() {
	//GPSRead();
	/*
	String data = "";
	data += "Valores Atmosfericos: " + BME_ReadData() + ". ";
	data += "Calidad del aire: " + readMQSensor() + ". ";
	data += "GPS: " + printGPSData() + " ";
	data += "Magneto: " + magReadData();
												
	PrintToOLED("Enviando Datos: " + data);
	LoRa_Send(data); */

	if (i == 0) {
		alt_init = getAltitude();
		i++;
	}

	String alt_init_str = "Altitud Inicial: " + String(alt_init) + " m s. n. m. ";
	String alt_act = "Altitud Actual: " + String(max(alt_base, getAltitude() - alt_init)) + " m.";


	PrintTwoLines("Datos Atmosfericos", alt_init_str, alt_act);
	LoRa_Send("Datos Atmosfericos~Alura Inicial: " + String(alt_init) + " m s. n. m.~" + alt_act + "~");
	delay(100);

	PrintTwoLines(getHumidity(), getPressure(), getTemperature());
	LoRa_Send(getHumidity() + "~" + getPressure() + "~" + getTemperature() + "~");
	delay(100);

	PrintTwoLines("Calidad del aire: ", ValorMQSensor() + " / 300", readMQSensor());
	LoRa_Send("Calidad del aire:~" + ValorMQSensor() + " / 300~" + readMQSensor() + "~");
	delay(100);


}
