#include <Wire.h>
#include <SSD1306Wire.h>

// Configuración OLED (Heltec V2)
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16

extern SSD1306Wire display;

void OLED_Init(void);
void PrintToOLED(String texto);

