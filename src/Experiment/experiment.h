#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <Arduino.h>
#include <ArduinoJson.h>


class Experiment {
  private:
    String name;
    String desc;
    int posVar = 0;
    int posVarSensor = 0;
    StaticJsonDocument<256> doc;
    JsonArray sensors;
    JsonArray variables;
    

  public:
    static const int UPDATE_VAR;
    static const int GET_STATE;
	static const int SAVE_VALUES;
	
    Experiment(String name);

    template <typename T>
    void addVariable(String name, String label, T defaultValue);
	template <typename T>
    void updateVariable(String name, T newValue);
	
    void addSensor(int pin, String name, int value);
	void processUpdateRequest();
	void writeVariablesToEeprom();
	void readEepromToVariables();
    void updateSensors();
    template <typename T>
    T getValue(String name);
    void updateExperiment();
	int getSensorValue(String sensorName);
    StaticJsonDocument<256> getDoc();   
};
#endif
