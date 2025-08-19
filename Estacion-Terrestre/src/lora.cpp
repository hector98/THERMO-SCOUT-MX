#include "lora.h"

void LoRa_Init(void) {
	LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);

	if (!LoRa.begin(433E6)) {
		//PrintToOLED("LoRa fallo");
		while (1);
	}

	LoRa.setTxPower(20);
	LoRa.setSpreadingFactor(7);

	//PrintToOLED("LoRa OK");
	LoRa_Receive();
	
	LoRa.receive();
}

void LoRa_Send(String message) {
	LoRa.beginPacket();
	LoRa.print(message);
	LoRa.endPacket();
}

String LoRa_Receive() {
	int packetSize = LoRa.parsePacket();
	if (packetSize) {
		String message = "";
		while (LoRa.available()) {
			message += (char)LoRa.read();
			PrintToOLED(message);
		}
		PrintToOLED("Recibiendo de LoRa: " + message);
		return message;
	}
	else {
		return "Sin mensaje";
	}
}

void LoRa_Data() {
	int packetSize = LoRa.parsePacket();
	if (packetSize) {
		String message = "";
		while (LoRa.available()) {
			message += (char)LoRa.read();
		}
		PrintToOLED("Recibiendo de LoRa: " + message);
	}
}


