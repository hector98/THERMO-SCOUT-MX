#include <Wire.h>
#include <SSD1306Wire.h>
#include <LoRa.h>
#include "lora.h"
#include "oled.h"

SSD1306Wire display(0x3c, OLED_SDA, OLED_SCL);

void setup() {
	Serial.begin(9600);
	
	OLED_Init(); // Pantalla
				 
	// Inicializaciones
	LoRa_Init(); // LoRa

	PrintThreeLines("Thermo Scout MX", "Estacion Terrestre", "Iniciando comunicacion LoRa");
}

String datas[3];
int i = 0;
int j = 0;

void loop() {

	LoRa_Data();

	delay(10);
	
	/*
	Serial.println("Esperando datos...");
	String data = LoRa_Data();
	LoRa_Send("OK");
	PrintToOLED("Esperando datos...");
	PrintToOLED(data);

	Serial.println(data);
	*/
}
