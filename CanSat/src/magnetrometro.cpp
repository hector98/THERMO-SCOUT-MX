#include "magnetrometro.h"

QMC5883LCompass compass;
															   
void Magnetrometro_Init(void){
	Wire.begin(SDA, SCL);

	compass.init();
	//PrintToOLED("Magnetrometro Inicializado");
}

String getMagnetometro(void){
	compass.read();

	String data = "Magnetrómetro X: " + compass.getX();
	data += "~";
	data += " Magnetrómetro Y: " + compass.getY();
	data += "~";
	data += " Magnetrómetro Z: " + compass.getZ();
	return data;
}
