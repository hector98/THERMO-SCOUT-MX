#include "mq_sensor.h"

String ValorMQSensor(void){
	int rawValue = analogRead(MQSensor);       // Lectura de 0-4095
	float calidad = rawValue *  0.073;

	return String(calidad);
}

String readMQSensor(void){
	int rawValue = analogRead(MQSensor);       // Lectura de 0-4095
	float voltage = (rawValue * 3.3) / 4095.0;  // Convertir a voltaje (0-3.3V)
	float calidad = rawValue *  0.073;

	String calidadStr = "";
	if (calidad < 50) {
		calidadStr = "ICA Bueno";
	} else if (calidad < 100) {
		calidadStr = "ICA Moderado";
	} else if (calidad < 150) {
		calidadStr = "ICA Desvaborable para grupos sensibles";
	} else if (calidad < 200) {
		calidadStr = "ICA Dañino para la salud";
	} else {
		calidadStr = "ICA Muy dañino para la salud";
	}

	return String(calidad);

}
