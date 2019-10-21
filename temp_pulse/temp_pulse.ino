#define USE_ARDUINO_INTERRUPTS true
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PulseSensorPlayground.h>
#include <SoftwareSerial.h>
SoftwareSerial ArduinoUno(3,2);

//variable for temperature sensor
int temp_sensor = 5;       // Pin DS18B20 Sensor is connected to

float temperature = 0;      //Variable to store the temperature in
int lowerLimit = 34;      //define the lower threshold for the temperature
int upperLimit = 38;      //define the upper threshold for the temperature

//  Variables
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 
float maxi = 0;

OneWire oneWirePin(temp_sensor);
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

DallasTemperature sensors(&oneWirePin);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sensors.begin();
  ArduinoUno.begin(4800);

  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Requesting Temperatures from sensors: ");
  sensors.requestTemperatures(); 
  Serial.println();
  
  temperature = sensors.getTempCByIndex(0);

  if(maxi < temperature){
    maxi = temperature;
    Serial.print("Temperature is: ");
    Serial.println(temperature);
    ArduinoUno.println(temperature);
  }

  /*if(temperature <= lowerLimit)
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
  }*/
  delay(1000);

  int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 

  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
    Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
    Serial.print("BPM: ");                        // Print phrase "BPM: " 
    Serial.println(myBPM);                        // Print the value inside of myBPM. 
    ArduinoUno.println(myBPM);
  }

  Serial.print("-----------------------------------");
  Serial.println("\n");
  delay(20);                    // considered best practice in a simple sketch.

}
