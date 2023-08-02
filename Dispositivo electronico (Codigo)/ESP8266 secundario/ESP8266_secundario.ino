// tiempo para letura de datos y envio
int periodo = 5000;
unsigned long tiempoAnterior = 0;
  #include <Wire.h>
  #include "MAX30100_PulseOximeter.h"
  #define REPORTING_PERIOD_MS     3000
  // PulseOximeter is the higher level interface to the sensor
  // it offers:
  //  * beat detection reporting
  //  * heart rate calculation
  //  * SpO2 (oxidation level) calculation
  PulseOximeter pox;
  uint32_t tsLastReport = 0;
  // Callback (registered below) fired when a pulse is detected
  void onBeatDetected()
  {
   // Serial.println("Beat!");
  }
int suma=0;
int suma2=0;
int suma3=0;
int HeartRate2;
int HeartRate3;
int spo22;
int spo23;
int con=0;
//Datos de filtro pasa bajo (Media móvil exponencial)
//An=a*M+(1-a)*An
//alpha de 0 a 1: 1 no filtra nada/ 0 filtra todo
//alpha clasico 0.05

float adc_filtrado = 0;
int adc_filtrado2 = 0;
int adc_raw1 = 0;
int adc_raw2 = 0;
#define alpha 0.05

void setup()
{
  Serial.begin(115200);
  
  Serial.print("Iniciando...");
  if (!pox.begin()) {
    Serial.println("MAX30100 no fue encontrado. Chequea la alimentación.");
    for (;;);
  } else {
    Serial.println("MAX30100 en funcionamiento..");
  }
  pox.setOnBeatDetectedCallback(onBeatDetected);
}
void loop()
{
 
  float HeartRate = 3.0, SpO2 = 3.0;
  // Make sure to call update as fast as possible
  pox.update();
 

  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
     HeartRate = pox.getHeartRate();
     SpO2 = pox.getSpO2();
     HeartRate2=HeartRate;
     spo22=SpO2;
     suma=suma+HeartRate2;
     suma2=suma2+spo22;
     con=con+1;
     tsLastReport = millis();
   }

if(millis()-tiempoAnterior>=periodo){
  tiempoAnterior=millis();
  HeartRate3=suma/con;
  spo23=suma2/con;
  Serial.print(HeartRate3);
  Serial.print(",");
  Serial.println(spo23);
  suma=0;
  suma2=0;
  con=0;
  }
 
}
