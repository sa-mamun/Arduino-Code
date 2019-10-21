#include <SoftwareSerial.h>
SoftwareSerial esp8266(2,3);  //rx,tx

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
esp8266.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:

if (esp8266.available()){

  while(esp8266.available()){
    char c=esp8266.read();
    Serial.write(c);
  }
}

if(Serial.available()){

  delay(1000);
  String command="";
  while(Serial.available()){
    command+=(char)Serial.read();
    
  }
  esp8266.println(command);
}
}
