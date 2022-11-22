#include "experiment.h"
#include <EEPROM.h>
#include <ArduinoJson.h>

const int Experiment::UPDATE_VAR = 0;
const int Experiment::GET_STATE = 1;
const int Experiment::SAVE_VALUES = 2;

template <typename T_ty> struct TypeInfo { static const char * name; };
template <typename T_ty> const char * TypeInfo<T_ty>::name = "unknown";

#define TYPE_NAME(var) TypeInfo< typeof(var) >::name
#define MAKE_TYPE_INFO(type)  template <> const char * TypeInfo<type>::name = #type;

MAKE_TYPE_INFO( int )
MAKE_TYPE_INFO( float )
MAKE_TYPE_INFO( boolean )

Experiment::Experiment(String name){
    this->doc["name"] = name;
	this->posVar = 0;
	this->posVarSensor = 0;
	this->readEepromToVariables();  
}

template <typename T>
void Experiment::addVariable(String name, String label,  T value){
    this->doc["variables"][this->posVar]["name"]  = name;
    this->doc["variables"][this->posVar]["value"] = value;
    this->doc["variables"][this->posVar]["label"] = label;  
	this->doc["variables"][this->posVar]["type"]  = TYPE_NAME(value);
    this->posVar++;
}

template <typename T>
void Experiment::updateVariable(String name, T newValue){
    for (int i=0; i < this->posVar; i++){
		if (this->doc["variables"][i]["name"] == name){
			this->doc["variables"][i]["value"] = newValue;
		}
    }
}

void Experiment::addSensor(int pin, String name, int value){
    this->doc["sensors"][this->posVarSensor]["name"] = name;
    this->doc["sensors"][this->posVarSensor]["value"] = value; 
    this->doc["sensors"][this->posVarSensor]["pin"] = pin;
    this->posVarSensor++;
}

int Experiment::getSensorValue(String sensorName){
	for (int i=0; i < this->posVarSensor; i++){
		if (this->doc["sensors"][i]["name"] == sensorName ){
			return this->doc["sensors"][i]["value"];
		}
    }
}

void Experiment::updateSensors(){
    for (int i=0; i < this->posVarSensor; i++){
		this->doc["sensors"][i]["value"] = analogRead(this->doc["sensors"][i]["pin"]);
    }
}

template <typename T>
T Experiment::getValue(String name){
    for (int i=0; i < this->posVar; i++){
		if (this->doc["variables"][i]["name"] == name){
			return this->doc["variables"][i]["value"];
		}
    }
}

void Experiment::writeVariablesToEeprom(){
  bool isEepromWriten = true; //if values were saved to EEPROM
  int posAddr = 0;
	
  EEPROM.put(posAddr, isEepromWriten);
  posAddr += sizeof(bool);

  for (int i=0; i < this->posVar; i++){
    if(this->doc["variables"][i]["type"] == "checkbox"){
      bool varValue = this->doc["variables"][i]["value"];
      EEPROM.put(posAddr, varValue);
      posAddr += sizeof(bool);
    }else if (this->doc["variables"][i]["type"] == "range"){
      int varValue = this->doc["variables"][i]["value"];
      EEPROM.put(posAddr, varValue);
      posAddr += sizeof(int);
    }
  }	
}

void Experiment::readEepromToVariables(){
  bool isEepromWriten; //if values were saved to EEPROM
  int posAddr = 0;
  EEPROM.get(posAddr, isEepromWriten);
  posAddr += sizeof(bool);
  if (isEepromWriten){
    for (int i=0; i < this->posVar; i++){		
      if(this->doc["variables"][i]["type"] == "checkbox"){
        bool varValue;
		EEPROM.get(posAddr, varValue);
        this->doc["variables"][i]["value"] = varValue;
        posAddr += sizeof(bool);
      }else if (this->doc["variables"][i]["type"] == "range"){
        int varValue;
        EEPROM.get(posAddr, varValue);
        this->doc["variables"][i]["value"] = varValue;
        posAddr += sizeof(int);
      }
    }
  }
}

StaticJsonDocument<256> Experiment::getDoc(){
    return this->doc;
}

void Experiment::updateExperiment(){
  StaticJsonDocument<256> com;
  this->updateSensors();
  
  DeserializationError err = deserializeJson(com, Serial);
  if (!err){
	switch (com["action"].as<int>()){
	  case 0:
		serializeJson(this->getDoc(), Serial);
        Serial.println("/EOJO");
		break;
	  case 1:
	    if(com["variable"]["type"].as<String>() == "boolean"){
			this->updateVariable<boolean>(com["variable"]["name"], com["variable"]["value"]);
		}else if (com["variable"]["type"] == "int"){
			this->updateVariable<int>(com["variable"]["name"], com["variable"]["value"]);
		}
		break;
	  case 2:
	    this->writeVariablesToEeprom();
	    break;
	  default:
	    break;
	}
  }
}
