//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

//------- Defining RFID Sensor---------
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN D4
#define RST_PIN D2

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "nodemcu-701e1.firebaseio.com"
#define FIREBASE_AUTH "5zOWXWjTb7evNPRV3FmPYHHsHPvWp8psbJsHV8sm"
#define WIFI_SSID "BAD BOYZ"
#define WIFI_PASSWORD "Mk011143006"

//---------For RFID--------------------
MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  //-------For RFID--------------------
  SPI.begin();       // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  Serial.println("RFID reading UID");
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int n = 0;
String str = "sam";

void loop() {
  //---------------RFID Start------------------
  String id = "";
  if ( mfrc522.PICC_IsNewCardPresent())
      {
          if ( mfrc522.PICC_ReadCardSerial())
          {
             Serial.print("Tag UID:");
             
             for (byte i = 0; i < mfrc522.uid.size; i++) {
                    //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                    //Serial.print(mfrc522.uid.uidByte[i], DEC);
                    id += (mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ") + String(mfrc522.uid.uidByte[i], HEX);
              }
            
              Serial.println(id);
              mfrc522.PICC_HaltA();

              // append a new value to /logs
              String name = Firebase.push(id,n++);
              // handle error
              /*if (Firebase.failed()) {
                  Serial.print("pushing /logs failed:");
                  Serial.println(Firebase.error());  
                  return;
              }*/
              Serial.print("pushed: ");
              Serial.println(name);
              delay(1000);
              Firebase.setFloat(String(id + "/temp"), 43.0);
              delay(1000);
          }
  }

//-------------RFID End------------------------

  
}
