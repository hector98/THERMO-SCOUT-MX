#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"  // Librería para microSD integrada
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// Pines del módulo AI-Thinker
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define FLASH_LED_PIN      4   // LED flash

int photo_counter = 0;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Evita reseteos por voltaje bajo
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  // Configuración de la cámara
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Resolución
  if(psramFound()){
    config.frame_size = FRAMESIZE_SVGA; // 800x600
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_VGA;  // 640x480
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Inicializar cámara
  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Error inicializando la cámara");
    return;
  }

  // Inicializar microSD
  if (!SD_MMC.begin()) {
    Serial.println("Error montando la microSD");
    return;
  }
  Serial.println("MicroSD lista");

  // Configurar flash
  pinMode(FLASH_LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(FLASH_LED_PIN, HIGH); // Enciende flash
  delay(50);                         // Pequeño retardo para iluminar

  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Error capturando la foto");
    return;
  }

  // Nombre del archivo
  String path = "/foto" + String(photo_counter++) + ".jpg";
  fs::FS &fs = SD_MMC;
  File file = fs.open(path.c_str(), FILE_WRITE);
  if (!file) {
    Serial.println("Error abriendo el archivo en la SD");
  } else {
    file.write(fb->buf, fb->len); // Guardar foto
    Serial.printf("Foto guardada: %s\n", path.c_str());
  }
  file.close();

  esp_camera_fb_return(fb); // Liberar memoria
  digitalWrite(FLASH_LED_PIN, LOW);  // Apaga flash

  delay(500); // Espera 500 ms
}

