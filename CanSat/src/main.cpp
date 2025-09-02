#include <Wire.h>
#include <SSD1306Wire.h>
#include <LoRa.h>
#include "oled.h"
#include "lora.h"
#include "gps.h"
#include "bme.h"
#include "magnetrometro.h"
#include "mq_sensor.h"
#include "acelerometro.h"

//Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345); // ID único
SSD1306Wire display(0x3c, OLED_SDA, OLED_SCL);

TinyGPSPlus gps;

Adafruit_BME280 bme;

void setup() {
	Serial.begin(115200);
	pinMode(Vext, OUTPUT);
	digitalWrite(Vext, LOW);
	delay(100);

	Serial.println("Iniciando CanSat");

	// Inicializar OLED
	OLED_Init();
	//PrintToOLED("Thermo Scout MX");
	//PrintToOLED("CanSat");
	PrintTwoLines("ING Electronica", "Thermo Scout MX", "CanSat");

	//magInit();

	LoRa_Init(); // Inicializar LoRa

	GPS_Init(); // Inicializar GPS

	BME_Init(); // Inicializar BME

	Acelerometro_Init(); // Inicializar acelerometro
						 
	Magnetrometro_Init(); // Inicializar magnetrometro

	PrintTwoLines("BME280 OK", "LoRa OK", "GPS OK");
	Serial.println("Iniciando CanSat todo OK");

}

int i = 0;
float alt_init = 0;
float alt_base = 0;
unsigned long previousMillis = 0;
const long interval = 10000;

void loop() {
    unsigned long currentMillis = millis();

    // Leer datos del GPS de forma asíncrona en cada ciclo
    while (Serial2.available() > 0) {
        gps.encode(Serial2.read());
    }

    if (currentMillis - previousMillis >= interval) {

        PrintOnOLED("Enviando Datos");
		delay(20);

        previousMillis = currentMillis;

        String data = "";
        Serial.println("Enviando Datos");

        if (i == 0) {
            alt_init = getAltitude();
            i++;
        }

        // Altitud del CanSat
        String alt_init_str = String(alt_init) + " m s. n. m. ";
        String alt_act = String(max(alt_base, getAltitude() - alt_init)) + " m.";
        data += "Altitud nivel del mar: " + alt_init_str + "~";
        data += "Altitud actual: " + alt_act + "~";

		LoRa_Send(data);
		data = "";
		delay(20);

        // Datos Atmosféricos
        data += getHumidity() + "~";
        data += getPressure() + "~";
        data += getTemperature() + "~";

		LoRa_Send(data);
		data = "";
		delay(20);

        // Calidad del aire
        data += "Calidad del aire:" + String(25) + "~";
		LoRa_Send(data);
		data = "";
		delay(20);

        // Magnetómetro
        data += getMagnetometro() + "~";
		LoRa_Send(data);
		data = "";
		delay(20);

        // Acelerómetro
        data += getAcelerometro() + "~";
		LoRa_Send(data);
		data = "";
		delay(20);

        // Localización GPS
        data += getGPSLocation() + "~";
		LoRa_Send(data);
		data = "";
		delay(20);

        // Liberación y paracaídas
        data += "Liberado: False~";
        data += "Paracaidas: False~";
		LoRa_Send(data);
		data = "";
		delay(20);

        PrintOnOLED("Enviando Datos");
		delay(20);
    }
}
