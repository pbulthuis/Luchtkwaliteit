#include "dht.h" // Toevoegen van de DHT library aan de code
#define dht_apin A0 // Analoge pin waarop de sensor is aangesloten

#include <SD.h>                                  //The libraries we need
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"

#include "Adafruit_CCS811.h"
Adafruit_CCS811 ccs;

RTC_DS1307 RTC;

const int chipSelect = 10;                        //Maybe not yours!!.You have to put your cs pin here
File Datos;                                      //Name of the file to be written to the sd

dht DHT;

int blauw = 4;
int rood = 5;
int geel = 6;
int groen = 7;

void setup(){
pinMode(10 , OUTPUT);    
Serial.begin(9600); //Start de seriele communicatie van de arduino met de pc                    
SD.begin(chipSelect);
Wire.begin();
RTC.begin();   


Datos=SD.open("Datos.csv",FILE_WRITE);           //Will open and write once just for headers
Datos.println("Date,Hour,Minute,Second,Humidity,Temperature,CO2 ppm,TVOC ppb");       //Print headers (not saved yet)     
Datos.close();                                   //Print saved

 Serial.println("CCS811 Reading CO2 and VOC");
  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  while(!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);

  RTC.adjust(DateTime(__DATE__, __TIME__));

  if (! RTC.isrunning())
  {
    Serial.println("RTC is NOT running!");
  }
  
Serial.println("DHT11 Humidity & temperature Sensor");

pinMode(rood, OUTPUT);
pinMode(geel, OUTPUT);
pinMode(groen, OUTPUT);

delay(1000); //Wacht seconde om op te starten
}

void loop(){

if(ccs.available()){
    float temp = ccs.calculateTemperature();
    if(!ccs.readData()){
      Serial.println("CO2: "+String(ccs.geteCO2())+"ppm, TVOC: "+String(ccs.getTVOC())+"ppb   Temp:"+String(temp));
    }
    else{
      Serial.println("Sensor read ERROR!");
      ccs.readData();
    }
  }


DHT.read11(dht_apin); //Leest de sensor op A0
Serial.print("Current humidity = "); //print tekst in de seriele monitor
Serial.print(DHT.humidity); //print luchtvochtigheidwaarde in monitor
Serial.print("% "); //print tekst % in seriele monitor
Serial.print("temperature = "); //print tekst in de seriele monitor
Serial.print(DHT.temperature); //print temperatuurwaarde
Serial.println("C ");//print tekst C in de seriele monitor en start nieuwe regel

DateTime now = RTC.now();

Datos = SD.open("Datos.csv", FILE_WRITE);       //Will open and write according to delay
Datos.print(now.year(), DEC);                //Print date and time(not saved yet)
Datos.print("/");
Datos.print(now.month(), DEC);
Datos.print("/");
Datos.print(now.day(), DEC);
Datos.print(",");
Datos.print(now.hour(), DEC);
Datos.print(",");
Datos.print(now.minute(), DEC);
Datos.print(",");
Datos.print(now.second(), DEC);
Datos.print(",");
Datos.print(DHT.humidity);
Datos.print(",");
Datos.print(DHT.temperature);
Datos.print(",");
Datos.print(""+String(ccs.geteCO2())+"");
Datos.print(",");
Datos.print(""+String(ccs.getTVOC())+"");
Datos.println();                               
Datos.close();                                  //Mesage saved

if (DHT.temperature < 23){
  digitalWrite(blauw, HIGH);}
  else if (DHT.temperature >= 23){
    digitalWrite(blauw, LOW);
    } 

if (DHT.temperature >= 23){
  digitalWrite(rood, HIGH);}
  else if (DHT.temperature < 23){
    digitalWrite(rood, LOW);  }





delay(1000);                                    
}