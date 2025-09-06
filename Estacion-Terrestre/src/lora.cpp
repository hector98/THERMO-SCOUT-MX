#include "lora.h"

void LoRa_Init(void) {
	LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);

	if (!LoRa.begin(915E6)) {
		//PrintToOLED("LoRa fallo");
		while (1);
	}


	LoRa.setFrequency(915E6);
	LoRa.setSpreadingFactor(12);       // SF12 (máximo alcance)
	LoRa.setSignalBandwidth(62.5E3);   // BW = 62.5 kHz (menos ancho = más alcance)
	LoRa.setCodingRate4(8);            // CR = 4/8 (mayor redundancia)
	LoRa.setTxPower(20);               // Potencia máxima (20 dBm)
									   
	//PrintToOLED("LoRa OK");
	//LoRa_Receive();
	
	LoRa.receive();
}

void LoRa_Send(String message) {
	LoRa.beginPacket();
	LoRa.print(message);
	LoRa.endPacket();
}

void LoRa_Data() {
	int packetSize = LoRa.parsePacket();
	if (packetSize) {
		String message = "";
		while (LoRa.available()) {
			message += (char)LoRa.read();
		}
		Serial.println(message);
		OnlyPrintToOLED("Recibiendo de LoRa: ");
		delay(10);
	}
	else {
		OnlyPrintToOLED("Esperando datos...");
	}
}


