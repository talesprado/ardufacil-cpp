/*
 * 
 * This example creates an experience to simulate a light pool.
 * The idea is that a student evaluates situacions where the light is on
 * like when it is night or raining but only if energy is running.
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
  xp.addVariable<boolean>("var2", "Noite", false);
  xp.addVariable<boolean>("var3", "Chovendo", false);
  //adds a sensor to be shown in the web interface
  xp.addSensor(A2, "Fotovoltaico", 0);  
}

void loop() {
  xp.updateExperiment();
  if  (xp.getValue<boolean>("var1") and (xp.getValue<boolean>("var2") or xp.getValue<boolean>("var3") ) ){
    digitalWrite(LED_BUILTIN, HIGH);
  }else{
    digitalWrite(LED_BUILTIN, LOW);
  }
}