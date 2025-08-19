#include "magnetrometro.h"
																   
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(); // ID único
														   //
void magInit(void){

	if (!mag.begin()) {
		//PrintToOLED("¡Error al detectar el HMC5883L!");
		while (1);
	}
	//PrintToOLED("HMC5883L detectado correctamente");
}

String magReadData(void){
	String data = "";
	sensors_event_t event;
	mag.getEvent(&event);

	data += "X: " + String(event.magnetic.x) + " uT";
	data += "Y: " + String(event.magnetic.y) + " uT";
	data += "Z: " + String(event.magnetic.z) + " uT";

	return data;
}
