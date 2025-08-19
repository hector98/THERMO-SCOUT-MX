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
	//LoRa_Data();
	int packetSize = LoRa.parsePacket();
	if (packetSize) {
		String message = "";
		while (LoRa.available()) {
			message += (char)LoRa.read();
		}
		//PrintToOLED("Recibiendo de LoRa: " + message);

		String dato = "";
		while ( j < message.length()) {	
			if (message[j] != '~') {
				dato += message[j];
				j++;
			}
			else {
				datas[i] = dato;
				i++;
				dato = "";
				j++;
			}
		}
		i = 0;
		j = 0;
		//PrintToOLED("Recibiendo de LoRa: " + message);
		PrintThreeLines(datas[0], datas[1], datas[2]);
	}
	/*
	String data = LoRa_Receive();
	PrintToOLED("Recibiendo: " + data);

	if (i < 3) {
		datas[i] = data;
		i++;
	} else {
		PrintThreeLines(datas[0], datas[1], datas[2]);
		i = 0;
	}*/

	//delay(10);
}
