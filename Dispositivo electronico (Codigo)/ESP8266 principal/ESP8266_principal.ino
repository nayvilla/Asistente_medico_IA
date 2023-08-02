//Importacion de librerias
#include <cambiar_ssid.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h> 
#include <Wire.h>
#include <Adafruit_MLX90614.h>
//Librerias de pantalla oled
#define __DEBUG__
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
/////////////////////////////////////////////////////////////////////////////////
//Declaracion de variables
#define DEBUG_ARRAY(a) {for (int index = 0; index < sizeof(a) / sizeof(a[0]); index++)    {Serial.print(a[index]); Serial.print('\t');} Serial.println();};
const char separator = ','; // separador de trama
const int dataLength = 2; // el numero del incremento para los valores de la cadena
float data[dataLength]; // tipo de datos que se almacena en el vector
//Variables del broker mosquito
WiFiClient espClient;
PubSubClient client(espClient);
String str = "";
String interruptor;
const char* mqtt_server = "34.125.127.48";
//variables de sensores
int bpm, spo2;
long lastMsg = 0;
char msg[50];
int value = 0;
// Instanciar objeto (Sensor de temperatura)
Adafruit_MLX90614 termometroIR = Adafruit_MLX90614();
// variables para medi nivel de carga
int analogInPin  = A0;    // Declaracion pin analogo A0
int sensorValue;          // Variable para la medicion del nivel de carga
float calibration = 0.43; // Calibrar el valor usando un multimetro de acuerdo al voltaje de la bateria
int bat_percentage;       // variable del porcentaje de la bateria
// variables de pantalla oled
#define ancho_imagen 20
#define alto_imagen 20

#define ancho_imagen_2 50
#define alto_imagen_2 50

const unsigned char PROGMEM termometro[] = {
	0x00, 0x60, 0x00, 0x00, 0x90, 0x00, 0x00, 0x90, 0x00, 0x00, 0x90, 0x00, 0x00, 0x90, 0x00, 0x00, 
	0x90, 0x00, 0x00, 0x90, 0x00, 0x00, 0xb0, 0x00, 0x00, 0xb0, 0x00, 0x00, 0xb0, 0x00, 0x00, 0xb0, 
	0x00, 0x00, 0xb0, 0x00, 0x00, 0xb0, 0x00, 0x00, 0xb0, 0x00, 0x00, 0x90, 0x00, 0x01, 0x68, 0x00, 
	0x01, 0x68, 0x00, 0x01, 0x68, 0x00, 0x01, 0x08, 0x00, 0x00, 0xf0, 0x00
};

const unsigned char PROGMEM corazon[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x3f, 0x9f, 0xc0, 0x3f, 0xff, 0xc0, 0x7f, 
	0xff, 0xe0, 0x7f, 0xff, 0xe0, 0x7f, 0xff, 0xe0, 0x3f, 0xff, 0xc0, 0x3f, 0xff, 0xc0, 0x3f, 0xff, 
	0xc0, 0x1f, 0xff, 0x80, 0x0f, 0xff, 0x00, 0x07, 0xfe, 0x00, 0x03, 0xfc, 0x00, 0x01, 0xf8, 0x00, 
	0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM saturacion[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xa0, 0x00, 0x01, 
	0xb0, 0x00, 0x01, 0x98, 0x00, 0x01, 0xfc, 0x00, 0x00, 0xf4, 0x00, 0x01, 0xf6, 0x00, 0x03, 0x72, 
	0x00, 0x03, 0x7e, 0x00, 0x06, 0x3c, 0x00, 0x06, 0x30, 0x00, 0x06, 0x10, 0x00, 0x07, 0x30, 0x00, 
	0x03, 0xe0, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM termometro2[] = {
	0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x06, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x38, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x18, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x04, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
	0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x18, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x04, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xc8, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xc8, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x04, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xc8, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x04, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
	0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xc8, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x04, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xc8, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x04, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x04, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0xcc, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xc3, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x31, 0xe1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0xf1, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x63, 0xf1, 0x80, 0x00, 0x00, 0x00, 0x00, 0x63, 0xf1, 0x80, 0x00, 0x00, 0x00, 0x00, 0x63, 
	0xf1, 0x80, 0x00, 0x00, 0x00, 0x00, 0x21, 0xe1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x03, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x18, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x0e, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM corazon2[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0xff, 0xe0, 0x01, 
	0xff, 0xc0, 0x00, 0x01, 0xff, 0xf0, 0x03, 0xff, 0xe0, 0x00, 0x03, 0xff, 0xfc, 0x07, 0xff, 0xf0, 
	0x00, 0x0f, 0xff, 0xfc, 0x0f, 0xff, 0xfc, 0x00, 0x0f, 0xff, 0xfe, 0x1f, 0xff, 0xfe, 0x00, 0x1f, 
	0xff, 0xfe, 0x1f, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x00, 0x3f, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xfc, 0x7f, 0xff, 0xff, 0x80, 0x7f, 0xff, 0xfc, 0x7f, 0xff, 
	0xff, 0x80, 0x7f, 0xff, 0xf8, 0x7f, 0xff, 0xff, 0x80, 0x7f, 0xff, 0xf8, 0x7f, 0xff, 0xff, 0x80, 
	0x7f, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0x80, 0x7f, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0x80, 0x7f, 0xff, 
	0xf0, 0x3f, 0xff, 0xff, 0x80, 0x7f, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xe0, 0x1f, 
	0x1c, 0x00, 0x00, 0x3f, 0xff, 0xe1, 0x0f, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x21, 0x0e, 0x08, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x0e, 0x08, 0x7e, 0x00, 0x00, 0x00, 0x03, 0x86, 0x00, 0x7c, 0x00, 0x07, 
	0xfe, 0x03, 0x84, 0x00, 0xf8, 0x00, 0x03, 0xfe, 0x03, 0x84, 0x01, 0xf0, 0x00, 0x03, 0xff, 0x07, 
	0x84, 0x01, 0xf0, 0x00, 0x01, 0xff, 0x07, 0x84, 0x01, 0xe0, 0x00, 0x00, 0xff, 0x87, 0xc0, 0x61, 
	0xc0, 0x00, 0x00, 0x7f, 0x87, 0xc0, 0x63, 0x00, 0x00, 0x00, 0x3f, 0xcf, 0xe0, 0x7f, 0x00, 0x00, 
	0x00, 0x1f, 0xff, 0xe0, 0x7e, 0x00, 0x00, 0x00, 0x07, 0xff, 0xe0, 0xf8, 0x00, 0x00, 0x00, 0x03, 
	0xff, 0xf0, 0xf0, 0x00, 0x00, 0x00, 0x03, 0xff, 0xf0, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf8, 
	0xc0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf8, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
	0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM saturacion2[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
	0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x12, 0x0a, 0x80, 0x00, 0x00, 0x00, 0x00, 0x08, 0x14, 0x40, 0x00, 0x00, 0x00, 0x00, 0x06, 0x60, 
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x20, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x0c, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x10, 0x00, 0x00, 0x00, 
	0x00, 0x21, 0x38, 0x08, 0x00, 0x00, 0x00, 0x00, 0x21, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 
	0x98, 0x10, 0x00, 0x00, 0x00, 0x00, 0x80, 0x16, 0x10, 0x00, 0x00, 0x00, 0x00, 0x80, 0x4c, 0xe0, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x23, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 
	0x00, 0x02, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x10, 0x00, 0x00, 0x00, 0x00, 0x04, 0x80, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x10, 
	0x00, 0x00, 0x00, 0x00, 0x02, 0x80, 0x10, 0x00, 0x00, 0x00, 0x00, 0x02, 0x40, 0x10, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8e, 0x40, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x61, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM carga100[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 
	0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0x00, 0x00, 0x00, 
	0x00, 0x0f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x1c, 
	0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 
	0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x7f, 0xf8, 0x60, 0x00, 
	0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 
	0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0xff, 
	0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 
	0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 
	0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 
	0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0x7f, 0xf8, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 
	0x60, 0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 
	0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 
	0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0x7f, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 
	0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x7f, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 
	0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 
	0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0x7f, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 
	0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 
	0x60, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xe0, 0x00, 
	0x00, 0x00, 0x0f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM carga75[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x01, 0x7f, 0xf0, 0x00, 0x00, 0x00, 
	0x00, 0x0f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x1c, 
	0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 
	0xe0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0xe0, 0x00, 
	0x00, 0x00, 0x38, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 
	0x38, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x38, 0x00, 
	0x00, 0xe0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x38, 0x7f, 0xf0, 0xe0, 
	0x00, 0x00, 0x00, 0x38, 0xff, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0x38, 0xff, 0xf8, 0xe0, 0x00, 0x00, 
	0x00, 0x38, 0xff, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0x38, 0xff, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0x38, 
	0xff, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 
	0xe0, 0x00, 0x00, 0x00, 0x38, 0xff, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0x38, 0xff, 0xf8, 0xe0, 0x00, 
	0x00, 0x00, 0x38, 0xff, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0x38, 0xff, 0xf8, 0xe0, 0x00, 0x00, 0x00, 
	0x38, 0xff, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x38, 0x00, 
	0x00, 0xe0, 0x00, 0x00, 0x00, 0x38, 0xff, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0x38, 0xff, 0xf8, 0xe0, 
	0x00, 0x00, 0x00, 0x38, 0xff, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0x38, 0xff, 0xf8, 0xe0, 0x00, 0x00, 
	0x00, 0x38, 0xff, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0x38, 0xff, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0x38, 
	0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 
	0xe0, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xc0, 0x00, 
	0x00, 0x00, 0x0f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM carga50[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x00, 
	0x00, 0x0f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x1c, 
	0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 
	0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 
	0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 
	0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 
	0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 
	0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 
	0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 
	0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 
	0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 
	0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 
	0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0xff, 
	0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 
	0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 
	0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 
	0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 
	0x60, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0xe0, 0x00, 
	0x00, 0x00, 0x1f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 
	0x03, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM carga25[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 
	0x00, 0x0f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x3c, 
	0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 
	0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 
	0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 
	0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 
	0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 
	0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 
	0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 
	0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 
	0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 
	0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 
	0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 
	0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0xff, 0xf0, 0xc0, 
	0x00, 0x00, 0x00, 0x31, 0xff, 0xf8, 0xc0, 0x00, 0x00, 0x00, 0x31, 0xff, 0xf8, 0xc0, 0x00, 0x00, 
	0x00, 0x31, 0xff, 0xf8, 0xc0, 0x00, 0x00, 0x00, 0x31, 0xff, 0xf8, 0xc0, 0x00, 0x00, 0x00, 0x31, 
	0xff, 0xf8, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 
	0xc0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x01, 0xc0, 0x00, 
	0x00, 0x00, 0x1f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char PROGMEM carga0[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 
	0x00, 0x00, 0x00, 0x02, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 
	0x00, 0x3f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x30, 
	0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x07, 
	0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x07, 0x00, 0x00, 
	0x00, 0x00, 0x30, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 
	0x30, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 
	0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x02, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x22, 0x07, 0x00, 
	0x00, 0x00, 0x00, 0x30, 0x22, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x22, 0x07, 0x00, 0x00, 0x00, 
	0x00, 0x30, 0x7f, 0x87, 0x00, 0x00, 0x00, 0x00, 0x30, 0xff, 0x87, 0x00, 0x00, 0x00, 0x00, 0x30, 
	0x7f, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x7f, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x7f, 0x07, 
	0x00, 0x00, 0x00, 0x00, 0x30, 0x7f, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x3e, 0x07, 0x00, 0x00, 
	0x00, 0x00, 0x30, 0x0c, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0c, 0x07, 0x00, 0x00, 0x00, 0x00, 
	0x30, 0x0c, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0c, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0c, 
	0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0c, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0c, 0x07, 0x00, 
	0x00, 0x00, 0x00, 0x30, 0x08, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x07, 0x00, 0x00, 0x00, 
	0x00, 0x30, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 
	0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x07, 
	0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xfe, 0x00, 0x00, 
	0x00, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define ancho_centigrados 15
#define alto_centigrados 15

const unsigned char PROGMEM centigrados[] = {
	0x00, 0x00, 0x00, 0x00, 0x78, 0x1c, 0x88, 0x7e, 0x88, 0xc0, 0x49, 0x80, 0x01, 0x80, 0x01, 0x00, 
	0x01, 0x00, 0x01, 0x80, 0x01, 0x80, 0x00, 0xe0, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00
};
// Definir constantes
#define ANCHO_PANTALLA 128 // ancho pantalla OLED
#define ALTO_PANTALLA 64 // alto pantalla OLED
 
// Objeto de la clase Adafruit_SSD1306
Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, -1);
///////////////////////////////////////////////////////////////////////////// 
void setup() {
 conectarWiFi();
 oled();
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
  // Iniciar termómetro infrarrojo con Arduino
  termometroIR.begin();
}
 
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  while (Serial.available() > 0) {
 envioCondicion();
 }
 bpm=data[0]; // guardamos el dato en la variable BPM
 spo2=data[1]; // guardamos el dato en la variable SPO2
  // Obtener temperaturas grados Celsius
  float temperaturaAmbiente = termometroIR.readAmbientTempC();
 // se aumenta el valor de "6" para calibrar el valor de la temepratura del cuerpo
  float temperaturaObjeto = (termometroIR.readObjectTempC()+6);
 
  // Mostrar información
  
  String temperaturaObjeto2 = String(temperaturaObjeto,2);
  String bpms = String(bpm); 
  String spo22 = String(spo2);
  String sensores = temperaturaObjeto2+","+bpms+","+spo22;
 
  const char *sensores2= sensores.c_str();
  snprintf (msg, 75, sensores2);  
  client.publish("chatboot/sensores", msg);
 
  delay(500);
  ///Mostrar datos en pantalla oled
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE); // Color del texto
  display.setTextSize(1.5); // Tamaño del texto
  display.setCursor(31, 0);
  display.println("TEMPERATURA");  
  display.drawBitmap(0,15, termometro2, ancho_imagen_2, alto_imagen_2, WHITE); 
  display.setTextSize(2); // Tamaño del texto
  display.setCursor(38, 30);
  display.println(temperaturaObjeto);
  display.drawBitmap(100,30, centigrados, ancho_centigrados, alto_centigrados, WHITE);
  display.display(); // Enviar a pantalla
  delay(2000);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE); // Color del texto
  display.setTextSize(1.5); // Tamaño del texto
  display.setCursor(57, 0);
  display.println("BPM");  
  display.drawBitmap(0,15, corazon2, ancho_imagen_2, alto_imagen_2, WHITE); 
  display.setTextSize(2); // Tamaño del texto
  display.setCursor(70, 30);
  display.println(bpm);
  display.display(); // Enviar a pantalla
  delay(2000);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE); // Color del texto
  display.setTextSize(1.5); // Tamaño del texto
  display.setCursor(55, 0);
  display.println("SPO2");
  display.drawBitmap(0,15, saturacion2, ancho_imagen_2, alto_imagen_2, WHITE); 
  display.setTextSize(2); // Tamaño del texto
  display.setCursor(70, 30);
  display.println(spo2);
  display.setCursor(100, 30);
  display.println("%");
  display.display(); // Enviar a pantalla
  delay(2000);

  //Medir nivel de bateria
  sensorValue = analogRead(analogInPin);
  float voltage = (((sensorValue * 3.3) / 1024) * 2 + calibration); //multiplique por dos ya que la red divisora ​​de voltaje es 100K y 100K Resistor
  bat_percentage = mapfloat(voltage, 2.8, 4.72, 0, 100); //2.8V as Battery Cut off Voltage & 4.2V as Maximum Voltage

  if (bat_percentage>75){
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE); // Color del texto
    display.setTextSize(1.5); // Tamaño del texto
    display.setCursor(2, 0);
    display.println("CARGA DEL DISPOSITIVO");  
    display.drawBitmap(0,10, carga100, ancho_imagen_2, alto_imagen_2, WHITE); 
    display.setTextSize(4); // Tamaño del texto
    display.setCursor(50, 30);
    display.println(bat_percentage);
    display.setTextSize(1); // Tamaño del texto
    display.setCursor(120, 30);
    display.println("%");  
    display.display(); // Enviar a pantalla
    delay(2000);    
  }
  else if (bat_percentage>50 && bat_percentage<=75){
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE); // Color del texto
    display.setTextSize(1.5); // Tamaño del texto
    display.setCursor(2, 0);
    display.println("CARGA DEL DISPOSITIVO");  
    display.drawBitmap(0,10, carga75, ancho_imagen_2, alto_imagen_2, WHITE); 
    display.setTextSize(4); // Tamaño del texto
    display.setCursor(50, 30);
    display.println(bat_percentage);
    display.setTextSize(1); // Tamaño del texto
    display.setCursor(120, 30);
    display.println("%");  
    display.display(); // Enviar a pantalla
    delay(2000);    
  }
  else if (bat_percentage>25 && bat_percentage<=50){
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE); // Color del texto
    display.setTextSize(1.5); // Tamaño del texto
    display.setCursor(2, 0);
    display.println("CARGA DEL DISPOSITIVO");  
    display.drawBitmap(0,10, carga50, ancho_imagen_2, alto_imagen_2, WHITE); 
    display.setTextSize(4); // Tamaño del texto
    display.setCursor(50, 30);
    display.println(bat_percentage);
    display.setTextSize(1); // Tamaño del texto
    display.setCursor(120, 30);
    display.println("%");  
    display.display(); // Enviar a pantalla
    delay(2000);    
  }
  else if (bat_percentage>20 && bat_percentage<=25){
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE); // Color del texto
    display.setTextSize(1.5); // Tamaño del texto
    display.setCursor(2, 0);
    display.println("CARGA DEL DISPOSITIVO");  
    display.drawBitmap(0,10, carga25, ancho_imagen_2, alto_imagen_2, WHITE); 
    display.setTextSize(4); // Tamaño del texto
    display.setCursor(50, 30);
    display.println(bat_percentage);
    display.setTextSize(1); // Tamaño del texto
    display.setCursor(120, 30);
    display.println("%");  
    display.display(); // Enviar a pantalla
    delay(2000);    
  } 
  else {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE); // Color del texto
    display.setTextSize(1.5); // Tamaño del texto
    display.setCursor(2, 0);
    display.println("CARGA DEL DISPOSITIVO");  
    display.drawBitmap(0,10, carga0, ancho_imagen_2, alto_imagen_2, WHITE); 
    display.setTextSize(4); // Tamaño del texto
    display.setCursor(50, 30);
    display.println(bat_percentage);
    display.setTextSize(1.5); // Tamaño del texto
    display.setCursor(120, 30);
    display.println("%");  
    display.display(); // Enviar a pantalla
    delay(2000);
  }
 
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void oled() {
   // Iniciar pantalla OLED en la dirección 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
#ifdef __DEBUG__
    Serial.println("No se encuentra la pantalla OLED");
#endif
    while (true);
  }
  display.clearDisplay(); // Limpiar buffer
  display.setTextSize(1); // Tamaño del texto
  display.setTextColor(SSD1306_WHITE); // Color del texto
  display.setCursor(10, 32); // Posición del texto  
  display.println("CARGANDO ..."); // Escribir texto
  display.display(); // Enviar a pantalla
  ////
  delay(3000);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE); // Color del texto
  display.setCursor(30, 12);
  display.println("UTA - FISEI");
  display.setCursor(10, 28);
  display.println("TELECOMUNICACIONES");  
  display.setCursor(30, 44);
  display.println("2022 - 2023");
  display.setCursor(30, 55);
  display.setTextSize(0.5); // Tamaño del texto
  display.println("BY NAYTHAN");
  display.display(); // Enviar a pantalla
  delay(1000);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE); // Color del texto
  display.setTextSize(1); // Tamaño del texto
  display.setCursor(23, 5);
  display.println("SIGNOS VITALES");  
  display.drawBitmap(33,20, termometro, ancho_imagen, alto_imagen, WHITE);
  display.drawBitmap(55,20, corazon, ancho_imagen, alto_imagen, WHITE);
  display.drawBitmap(75,20, saturacion, ancho_imagen, alto_imagen, WHITE);  
  display.display(); // Enviar a pantalla
  delay(2000);
}
 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
 
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
 
}
 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("casa/comedor/temperatura", "Enviando el primer mensaje");
      // ... and resubscribe
      client.subscribe("casa/comedor/temperatura");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}
void envioCondicion(){
  str = Serial.readStringUntil('\n'); // leemos la cadena 
      for (int i = 0; i < dataLength ; i++) // inicializamos el for
      {
         int index = str.indexOf(separator); // tomamos valores en funcion del separador
         data[i] = str.substring(0, index).toFloat(); //lo convertimos en flotante
         str = str.substring(index + 1); // aumentamos el index 
      }  
}