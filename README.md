# Ardufacil C++
[![Create release](https://github.com/talesprado/ardufacil-cpp/actions/workflows/main.yml/badge.svg)](https://github.com/talesprado/ardufacil-cpp/actions/workflows/main.yml)

Este repositório contém os arquivos da biblioteca C++ para ser utilizada em programação Arduino.
Esta biblioteca oferece métodos para:
- adição de variáveis
- adição de sensores
- atualização de variávies
- recuperar valor de uma variável
- leitura de valores de sensores

## Para que utilizar esta biblioteca
Ao utilizar os métodos presentes nesta biblioteca para adicionar variávieis e sensores, ela permitirá que os valores dos sensores sejam lidos e os valores das variáveis sejam atualizados por uma página web.

## Como utilizar esta biblioteca

1. Faça download dos arquivos desse repositório e os copie para a pasta de bibliotecas do Arduino
2. No seu sketch, importe a biblioteca e inicie instancie a classe como um objeto global:
 ```
 #include <ArduinoX.h>
 Experiment xp("Poste de luz");
 ```
 3. No método setup(), adicione as variáveis que podem ser parametrizadas e os sensores que serão exibitos na tela
 ```
 void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  while(!Serial) continue;
  xp.addVariable<boolean>("var1", "Energia eletrica", false);
  xp.addVariable<int>("var2", "Sensor", 432);
  xp.addSensor(A2, "Fotovoltaico", 0);  
}
 ```
 4. No método loop(), chame o método updateExperiment() para atualizar os valores lidos nos sensores e os valores presentes nas variáveis parametrizáveis. Este método 
 também escutará a porta serial para receber solicitações de atualização e escrita de valores para EEPROM.
 ```
 xp.updateExperiment();
  if  (xp.getValue<boolean>("var1") and (xp.getSensorValue("Fotovoltaico") < xp.getValue<int>("var2") ) ){
    digitalWrite(LED_BUILTIN, HIGH);
  }else{
    digitalWrite(LED_BUILTIN, LOW);
  }
 ```
 
 ## Assinaturas de métodos
 Os seguintes métodos públicos podem ser utilizados 
 
 ### addVariable
 Este método adiciona uma variável configurável ao objeto Experiment.
 ```
 template <typename T>
 void addVariable(String name, String label, T defaultValue);
 ```
 Como é possível ver na assinatura acima, este método utiliza um template para o tipo de dado do parâmetro defaultValue. 
 Isto significa que o tipo da variável configurável deve ser fornecido no momento que é o método é chamado como abaixo:
 ```
 xp.addVariable<boolean>("var1", "Energia eletrica", false);
 ```
 No momento apenas <int> e <boolean> são aceitos como parâmetros
 
 ### addSensor
 Este método adiciona um sensor ao objeto Experiment. Após adicionado, a leitura do valor do pino será feita dentro do método updateExperiment().
 ```
 void addSensor(int pin, String name, int value);
 ```
 
 ### updateExperiment
Este método escuta a porta serial para identificar se houve uma solicitação de valores das variáveis e sensores, uma solicitação para alterar o valor de algum dos 
 parâmetros do métodos ou uma solicitação para salvar os valores atuais na EEPROM.
 ```
 void updateExperiment();
 ```
