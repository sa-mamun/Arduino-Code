#include <SoftwareSerial.h>
SoftwareSerial ArduinoUno(3,4);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ArduinoUno.begin(4800);

}

void loop() {
  // put your main code here, to run repeatedly:
  /*while(ArduinoUno.available()>0){
    float val = ArduinoUno.parseFloat();
    if(ArduinoUno.read() == '\n'){
      Serial.println(val);
    }
  }
  delay(30);*/
  int i = 10;
  ArduinoUno.print(i);
  ArduinoUno.println("\n");
  delay(30);
}
