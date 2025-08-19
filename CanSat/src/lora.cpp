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
}

void LoRa_Send(String message) {
	LoRa.beginPacket();
	LoRa.print(message);
	LoRa.endPacket();
}
