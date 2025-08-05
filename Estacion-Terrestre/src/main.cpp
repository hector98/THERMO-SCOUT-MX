#include <Wire.h>
#include <SSD1306Wire.h>
#include <LoRa.h>
#include "lora.h"
#include "oled.h"

SSD1306Wire display(0x3c, OLED_SDA, OLED_SCL);

void setup() {
	Serial.begin(9600);
	
	OLED_Init(); // Pantalla
				 //
	// Mensaje de inicio
	PrintToOLED("Thermo Scout MX");
	PrintToOLED("Iniciando Estacion Terrestre");

	// Inicializaciones
	LoRa_Init(); // LoRa

	PrintToOLED("LoRa OK");
}

void loop() {
	LoRa_Receive();
	delay(10);
}
