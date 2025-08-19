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

void PrintThreeLines(String line1, String line2, String line3){
	int posX = 128;
	int velocidad = 2;
	int anchoLine1 = display.getStringWidth(line1);
	int anchoLine2 = display.getStringWidth(line2);
	int anchoLine3 = display.getStringWidth(line3);
	int anchoMax = anchoLine1;

	if (anchoLine2 > anchoMax) {
		anchoMax = anchoLine2;
	}else if (anchoLine3 > anchoMax) {
		anchoMax = anchoLine3;
	}

	while (posX > -anchoMax + 1) {
		display.clear();
		display.drawString(posX, 5, line1);
		display.drawString(posX, 25, line2);
		display.drawString(posX, 45, line3);
		posX -= velocidad;
		if (posX < -anchoMax) {
			posX = 128;
		}
		display.drawRect(0, 0, 128, 64);
		display.display();
		delay(50);
	}
}

