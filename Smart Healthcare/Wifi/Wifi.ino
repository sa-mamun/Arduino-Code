#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
SoftwareSerial NodeMCU(D2,D3);

String vall = "";
String temp = "";
String res = "";
char var;


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
  boolean check = false;
  while(NodeMCU.available()>0){
    /*float val = NodeMCU.parseFloat();
    if(NodeMCU.read() == '\n'){
      Serial.println(val);
    }*/
    var = NodeMCU.read();
    temp = String(var);
    vall += temp;
    check = true;
    /*if(NodeMCU.read() == '\n'){
      Serial.println(vall);
      vall = "";
    }*/
  }
  if(check){
    Serial.println(vall);
    res = vall;
    Serial.println("Here is the tag id: ");
    Serial.println(res);
    vall = "";
    String id = res.substring(0,11);
    String temp = res.substring(11,16);
    String time = res.substring(16,24);
    Serial.println(id);
    Serial.println(temp);
    Serial.println(time);
  }
  
  delay(1000);
}
