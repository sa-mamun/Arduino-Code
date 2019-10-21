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

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Set these to run example.
#define FIREBASE_HOST "nodemcu-701e1.firebaseio.com"
#define FIREBASE_AUTH "5zOWXWjTb7evNPRV3FmPYHHsHPvWp8psbJsHV8sm"
#define WIFI_SSID "BAD BOYZ"
#define WIFI_PASSWORD "Mk011143006"

int temp_sensor = 5;       // Pin DS18B20 Sensor is connected to

float temperature = 0;      //Variable to store the temperature in
int lowerLimit = 34;      //define the lower threshold for the temperature
int upperLimit = 38;      //define the upper threshold for the temperature


OneWire oneWirePin(temp_sensor);

DallasTemperature sensors(&oneWirePin);

void setup() {
  Serial.begin(9600);
  sensors.begin();
  
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
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int n = 0;

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Requesting Temperatures from sensors: ");
  sensors.requestTemperatures(); 
  Serial.println();
  
  temperature = sensors.getTempCByIndex(0);
  
  Serial.print("Temperature is ");
  Serial.print(temperature);
  //delay(1000);
  
  // set value
  Firebase.setFloat("Temp", temperature);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number: ");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // update value
  Firebase.setFloat("pulse", 72.00);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // get value 
  Serial.print("Temp: ");
  Serial.println(Firebase.getFloat("Temp"));
  Serial.println(Firebase.getFloat("pulse"));
  delay(1000);

  // remove value
  Firebase.remove("Temp");
  delay(1000);

  // set string value
  /*Firebase.setString("message", "hello world");
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // set bool value
  Firebase.setBool("truth", false);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);*/

  // append a new value to /logs
  String name = Firebase.pushInt("logs", n++);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(1000);
}
