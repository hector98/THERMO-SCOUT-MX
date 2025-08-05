#include "oled.h"

void OLED_Init(void){
	pinMode(OLED_RST, OUTPUT);
	digitalWrite(OLED_RST, LOW);
	delay(50);
	digitalWrite(OLED_RST, HIGH);
	
	display.init();
	display.flipScreenVertically();
	display.setFont(ArialMT_Plain_16);
}

void PrintToOLED(String texto){
	int posX = 128;
	int velocidad = 2;
	int anchoTexto = display.getStringWidth(texto);

	while (posX > -anchoTexto + 1) {
		display.clear();
		display.drawString(posX, 20, texto);
		posX -= velocidad;
		if (posX < -anchoTexto) {
			posX = 128;
		}
		display.drawRect(0, 0, 128, 64);
		display.display();
		delay(50);
	}
}
