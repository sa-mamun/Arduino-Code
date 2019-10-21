#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

SoftwareSerial NodeMCU(D2,D3);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  NodeMCU.begin(4800);
  pinMode(D2,INPUT);
  pinMode(D3,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  /*int i = 10;
  NodeMCU.print(i);
  NodeMCU.println("\n");
  delay(30);*/
  while(NodeMCU.available()>0){
    float val = NodeMCU.parseFloat();
    if(NodeMCU.read() == '\n'){
      Serial.println(val);
    }
    //Serial.println(val);
  }
  delay(30);
}
