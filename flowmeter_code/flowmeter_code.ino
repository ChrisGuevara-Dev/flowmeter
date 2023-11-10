#include <Wire.h>               //LCD
#include <LiquidCrystal_I2C.h>  //LCD
#include <OneWire.h>            //Temperatura     
#include <DallasTemperature.h>  //Temperatura  

const int sensorPin = 2;
const int measureInterval = 2500;
volatile int pulseConter;

int valvula=12;
int PinSensor=3;

int iniciar=10;int sta;
int parar=8; int sto;
int i=0;
int x=0;


OneWire ourWire(3);                //Se establece el pin 3  como bus OneWire
DallasTemperature sensors(&ourWire); //Se declara una variable u objeto para nuestro sensor


LiquidCrystal_I2C lcd(0x27,16,2);  //Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
 
// YF-S201
const float factorK = 7.5; 
 
void ISRCountPulse()
{
   pulseConter++;
}
 
float GetFrequency()
{

   pulseConter = 0;
 
   interrupts();
   delay(measureInterval);
   noInterrupts();
 
   return (float)pulseConter * 1000 / measureInterval;
}
 
void setup()
{

    
   lcd.init();
   lcd.backlight();
   Serial.begin(9600);
   attachInterrupt(digitalPinToInterrupt(sensorPin), ISRCountPulse, RISING);


   delay(500);
   pinMode(valvula, OUTPUT);
   pinMode(PinSensor, INPUT); 
   pinMode(iniciar, INPUT); 
   pinMode(parar, INPUT); 

   digitalWrite(valvula, HIGH);
    
   lcd.print("   Bienvenido   ");
   lcd.setCursor(0,1);
   lcd.print("Presione Start..");
   sta=digitalRead(iniciar);
   while(sta==HIGH){
    delay(100);
    sta=digitalRead(iniciar);
   }
   lcd.clear();
   

   sensors.begin();  //Se inicia el sensor de temperatura

   digitalWrite(valvula, LOW);
   lcd.setCursor(0,0);
   lcd.print("Q=");
   lcd.setCursor(7,0);
   lcd.print("L/min");
   lcd.setCursor(0,1);
   lcd.print("T=");
   lcd.setCursor(7,1);
   lcd.print("oC");
   
}
 
void loop()
{
   // obtener frecuencia en Hz
   float frequency = GetFrequency();
 
   // calcular caudal L/min
   float flow_Lmin = frequency / factorK;

    sensors.requestTemperatures();   //Se envía el comando para leer la temperatura
    float temp= sensors.getTempCByIndex(0); //Se obtiene la temperatura en ºC
 
   Serial.print("Frecuencia: ");
   Serial.print(frequency, 0);

   Serial.print(" (Hz)\tCaudal: ");
   Serial.print(flow_Lmin, 3);

   //lcd.setCursor(0,1);
   //lcd.print(flow_Lmin);
   
   Serial.print(" (L/min)\tTemperatura:");

   Serial.print(temp, 3);
   Serial.println(" (°C)");

   delay(500);
   
   lcd.setCursor(2,0);
   lcd.print(flow_Lmin);
   
   lcd.setCursor(2,1);
   lcd.print(temp);


   sto=digitalRead(parar);
    if(sto==LOW){
      i=1;
      digitalWrite(valvula,HIGH);
      lcd.clear();
      lcd.print("Sistema Pausado");
      lcd.setCursor(0,1);
      lcd.print("Presione reset..");
      while(i=1){      
      }
   }
   
}
