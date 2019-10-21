#include "SPI.h"
#include "MFRC522.h"
#include <SoftwareSerial.h>
#include <DS3231.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define REPORTING_PERIOD_MS     1000
#define SS_PIN 10
#define RST_PIN 9

int pulse = 0;
int oxygen;
int arr[30];
int average;
int sum = 0;
int i = 0;
String Time = "";
String puls = "";
String oxi = "";
int temp_sensor = 5;       // Pin DS18B20 Sensor is connected to
float temperature = 0;      //Variable to store the temperature in
float maxTemp = 0;

PulseOximeter pox;

uint32_t tsLastReport = 0;

DS3231  rtc(SDA, SCL);

OneWire oneWirePin(temp_sensor);

DallasTemperature sensors(&oneWirePin);

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

SoftwareSerial ArduinoUno(3,4);

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
    digitalWrite(7,HIGH);
    delay(10);
    digitalWrite(7,LOW);
    delay(10);
}


void setup()
{
    Serial.begin(115200);
    sensors.begin();
    ArduinoUno.begin(4800);
    SPI.begin();
    rfid.PCD_Init();
    rtc.begin();
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    
    // The following lines can be uncommented to set the date and time
    //rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
    //rtc.setTime(21, 53, 00);     // Set the time to 12:00:00 (24hr format)
    //rtc.setDate(12, 12, 2018);   // Set the date to January 1st, 2014
    Serial.print("Initializing pulse oximeter..");
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
    
}

void loop()
{
    // Make sure to call update as fast as possible
    pox.update();
    
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        pulse = pox.getHeartRate();
        oxygen = pox.getSpO2();

        if(pulse>30 && pulse<100){
          Serial.println(pulse);
          if(i<20){
            arr[i] = pulse; 
            i++; 
          }
          Serial.println(i);
        }
        
        if( i==20){
          for(int j=0;j<20;j++){
            sum = sum + arr[j];
          }
          average = sum/20;
          Serial.print("Average Pulse: ");
          Serial.println(average);
          puls = String(average);
          oxi = String(oxygen);
          //pulse = 0;
          //average = 0;
          //sum = 0;
          i++;
          digitalWrite(6,HIGH);
        }
          
        tsLastReport = millis();  
    }
    
    if(i >= 20){
      sensors.requestTemperatures(); 

      temperature = sensors.getTempCByIndex(0);
      
      if(maxTemp < temperature){
        maxTemp = temperature;
        Serial.print("Temperature is ");
        Serial.println(maxTemp);
        delay(50);
        pulse = 0;
      
      }

    //------------------For RFID Sensor----------------------//
      if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
        return;

      // Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      // Serial.println(rfid.PICC_GetTypeName(piccType));
    
      // Check is the PICC of Classic MIFARE type
      if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("Your tag is not of type MIFARE Classic."));
        return;
      }
    
      String strID = "";
      for (byte i = 0; i < 4; i++) {
        strID +=
        (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
        String(rfid.uid.uidByte[i], HEX) +
        (i!=3 ? ":" : "");
      }
      strID.toUpperCase();
    //-------------Date------------//
      
      //Date = rtc.getDateStr();
      Time = rtc.getTimeStr();
      
      strID += String(maxTemp) + Time + puls + oxi;
      maxTemp = 0.0;
      Serial.print("Tap card key: ");
      Serial.println(strID);
      ArduinoUno.print(strID);
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  
}
