#include <LoRa.h>
#include "oled.h"

#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_CS 18
#define LORA_RST 14
#define LORA_IRQ 26

void LoRa_Init(void);
void LoRa_Send(String message);
String LoRa_Receive(void);
void LoRa_Data(void);
