#Proyecto: THERMO-SCOUT MX
Sistema Académico de Monitoreo Atmosférico para
Prevención de Incendios Forestales en México
##1. Introducción y Relevancia en México
México enfrenta una creciente crisis de incendios
forestales, con un promedio de 7,400 incendios
anuales que afectan más de 260,000 hectáreas
(CONAFOR, 2023). El 90% son causados por
actividades humanas en zonas con condiciones
atmosféricas de alto riesgo. THERMO-SCOUT MX surge
como una solución académica innovadora para la
detección temprana de condiciones propicias para
incendios, utilizando tecnología accesible tipo
CanSat. Este proyecto, aunque no orbital, simula
una misión de monitoreo ambiental desde 1 km de
altura aproximadamente, con potencial para ser
escalado a redes de sensores terrestres en regiones
críticas como la Sierra Madre Occidental, Sierra
Madre Oriental o el Eje Neo-volcánico.
##2. Objetivos
Objetivo General:
Desarrollar un prototipo CanSat que monitoree
variables atmosféricas clave asociadas a incendios
forestales durante su descenso controlado,
generando datos para modelos predictivos locales.
Objetivos Específicos:
• Detección de Riesgo:
◦ Monitorear temperatura, humedad, CO₂, presión
y partículas PM₂.₅ en tiempo real.
◦ Identificar umbrales peligrosos (ej: Temp >
35°C + Humedad < 30% + CO₂ > 500 ppm).Análisis Espacial:
Capturar imágenes geoetiquetadas para evaluar
sequía vegetal mediante índices de color
(RGB/IR).
Educación Tecnológica:
Aprender a trabajar con tecnologías
Aeroespaciales en el Tecnológico Superior de
Fresnillo.
Validación:
Probar el sistema en zonas forestales del Estado
de Zacatecas (coordinación con Protección Civil,
y UMAS).
##3. Diseño Técnico con Wifi_LoRa_32_v2
Arquitectura Hardware:
Microcontrolador: ESP32 (por su bajo consumo,
ADC de 10 bits y puertos USB nativos).
Sensores:
BME280: Temperatura/Humedad/Presión (I²C).
MQ-135: Calidad del aire (CO₂, NOₓ) - Salida
analógica.
GPS Neo-6M: Altitud y coordenadas (UART).
Cámara OV7670: Captura de imágenes (controlada por
GPIO).Comunicación:
Módulo LoRa (SX1278) SPI (RF 433 MHz): Transmisión
de datos a 1 km (con antena omnidireccional).
Alimentación: Batería Li-Ion 3.7V + Regulador de 5
a 3.3 para modulo LoRa.
Recuperación: Paracaídas de seda o tela ligera
(activado por servo SG90 al detectar 800 m. de
altitud).
##4. Estación Terrestre y Software
Recepción de Datos:
Placa wifi_LoRa_32_v2 envía por USB a la PC.
Pantalla OLED 64x128 (SSD1306): Muestra en tiempo
real:
THERMO-SCOUT MX
Alt: 650m Temp: 34°C
CO2: 480ppm Riesgo: ALTO
Backend (Python):
• API con FastAPI y PySerial:
• Almacena en CSV:
fecha,hora,lat,lon,temp,co2,riesgo.
Frontend (React):
• Dashboard de Riesgo:
• Mapa interactivo (Leaflet) con capas de
calor.
• Gráficos de tendencia (Chart.js) para
temperatura/CO₂.• Clasificación automática.
5. Innovación y Aplicación en México
Prevención Adaptada al Contexto Mexicano:
Detección de "Zonas Críticas": Correlación de datos
con variables locales (tipo de vegetación, sequía
histórica).
Costo Accesible: Prototipo < $150 USD (ideal para
comunidades rurales).
Sinergias Institucionales: Datos compatibles con la
Plataforma Nacional de Información Forestal (PNIF).
Impacto Académico:
Modelo pedagógico para universidades (ej: UNAM,
IPN, UDG) en ingeniería aeroespacial aplicada.
Base para proyectos de ciencia ciudadana en
reservas naturales.
6. Conclusiones
THERMO-SCOUT MX demuestra que:
La tecnología de microsatélites educativos puede
adaptarse a problemas ambientales urgentes en
México.
La integración de hardware accesible (PIC18F4550)
con software de análisis predictivo genera
soluciones escalables.
Los datos obtenidos permitirán desarrollar modelos
de riesgo regionales, reduciendo falsas alarmas.
