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

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "nodemcu-701e1.firebaseio.com"
#define FIREBASE_AUTH "5zOWXWjTb7evNPRV3FmPYHHsHPvWp8psbJsHV8sm"
#define WIFI_SSID "BAD BOYZ"
#define WIFI_PASSWORD "Mk011143006"

SoftwareSerial NodeMCU(D2,D3);

String vall = "";
String temp = "";
String res = "";
char var;


void setup() {
  Serial.begin(115200);
  NodeMCU.begin(4800);
  pinMode(D2,INPUT);
  pinMode(D3,OUTPUT);

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

//---------------Wifi---------------------//
//---------------------------------------//
  boolean check = false;
  while(NodeMCU.available()>0){
    var = NodeMCU.read();
    temp = String(var);
    vall += temp;
    check = true;
  }
  if(check){
    Serial.println(vall);
    res = vall;
    Serial.println("Here is the tag id: ");
    Serial.println(res);
    vall = "";
    String id = res.substring(0,11);
    String temp = res.substring(11,16);
    Serial.println(id);
    Serial.println(temp);

    //-----------------------------------------//
    //-------------- Firebase -----------------//
  
    // append a new value to /logs
    Firebase.pushInt(id, n++);
    Firebase.setString(String(id + "/Temperature"), temp);
  }
  
  delay(1000);

}
