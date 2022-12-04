/*
 * 
 * This example creates an experience to simulate a light pool.
 * The idea is that a student identifies that the light in a light pole
 * is only turned on when a sensor reads few amount of light
 * 
 */
#include <ArduinoX.h>

//declares as global to accessed in both setup and loop methods
Experiment xp("Poste de luz");

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  while(!Serial) continue;
  //adds 3 variables that can be later redefined in the web interface
  xp.addVariable<boolean>("var1", "Energia elétrica", false);
  xp.addVariable<int>("var2", "Noite", false);
  //adds a sensor to be shown in the web interface
  xp.addSensor(A2, "Fotovoltaico", 0);  
}

void loop() {
  xp.updateExperiment();
  /*
  * Light will be turned on if var1 is true and the value defined in the web interface for var2 is higher 
  * than the value red in by the sensor Fotovoltaico
  */
  if  (xp.getValue<boolean>("var1") and (xp.getSensorValue("Fotovoltaico")  < xp.getValue<boolean>("var2") ) ){
    digitalWrite(LED_BUILTIN, HIGH);
  }else{
    digitalWrite(LED_BUILTIN, LOW);
  }
}