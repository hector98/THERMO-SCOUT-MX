#include "acelerometro.h"

Adafruit_MPU6050 mpu;

void Acelerometro_Init(void){
	Wire.begin(SDA, SCL);

	mpu.begin();

	mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
	mpu.setGyroRange(MPU6050_RANGE_500_DEG);
	mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

String getAcelerometro(void){
	String data = "";
	sensors_event_t a, g, temp;
	mpu.getEvent(&a, &g, &temp);

	data += "Acelerómetro X: " + String(a.acceleration.x);
	data += "~";
	data += " Acelerómetro Y: " + String(a.acceleration.y);
	data += "~";
	data += " Acelerómetro Z: " + String(a.acceleration.z);
	return data;
}
