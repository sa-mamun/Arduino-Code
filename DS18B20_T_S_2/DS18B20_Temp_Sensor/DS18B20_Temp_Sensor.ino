#include <OneWire.h>
#include <DallasTemperature.h>

int temp_sensor = 5;       // Pin DS18B20 Sensor is connected to

float temperature = 0;      //Variable to store the temperature in
int lowerLimit = 34;      //define the lower threshold for the temperature
int upperLimit = 38;      //define the upper threshold for the temperature


OneWire oneWirePin(temp_sensor);

DallasTemperature sensors(&oneWirePin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sensors.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Requesting Temperatures from sensors: ");
  sensors.requestTemperatures(); 
  Serial.println();
  
  temperature = sensors.getTempCByIndex(0);
  
  Serial.print("Temperature is ");
  Serial.print(temperature);

  if(temperature <= lowerLimit)
  {
    Serial.println();
    Serial.println("Body Temperature is LOW!!!");
  }
  else if(temperature > lowerLimit && temperature < upperLimit)
  {
    Serial.println();
    Serial.println("Body Temperature is NORMAL!!!");
  }
  else if(temperature >= upperLimit)
  {
    Serial.println();
    Serial.println("Body Temperature is HIGH!!!");
  }
  delay(1000);
}
