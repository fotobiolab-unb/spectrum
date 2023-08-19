// *********************** Caixinha para controle da válvula de injeção de CO2 ***********************

const byte CO2Pin = 41;
const byte Saida1Pin = 40;

boolean CO2_on = false;

void CO2On(){
  digitalWrite(CO2Pin, LOW);
  CO2_on = true;
}

void CO2Off(){
  digitalWrite(CO2Pin, HIGH);
  CO2_on = false;
}

// ***************************************** Sensor de CO2 ****************************************

#include "SparkFun_SCD30_Arduino_Library.h" 
SCD30 SensorCO2;

float Umidade = 666;
float GasTemp = 666;
float CO2 = 666;
int CO2Ref = 400;   // Variavel usada durante a calibração do sensor de co2

void lerGas(){   
  Umidade = SensorCO2.getHumidity();  
  delay(100);   
  GasTemp = SensorCO2.getTemperature(); 
  delay(500);     
  CO2 = SensorCO2.getCO2();  
} 

void calibrarCO2(){
  SensorCO2.setForcedRecalibrationFactor(CO2Ref);
}

// ***********************************************************************************************

unsigned long UltimoAjusteCO2 = 0;
byte dtCO2 = 30;
long CO2count = 0;
byte ModoCO2 = 0;           // 0: livre; 1: const co2
int MaxValveDelay = 2000;
int ValveDelay = 0;
int SetCO2 = 400;

void iniciarGas(){
  pinMode(CO2Pin, OUTPUT);
  CO2Off();
  pinMode(Saida1Pin, OUTPUT);
  digitalWrite(Saida1Pin, HIGH);

  SensorCO2.begin(); 
  SensorCO2.setAutoSelfCalibration(false);
  SensorCO2.setAltitudeCompensation(1030);
  Serial.println("Gas ok");
  
}

void ajustarCO2(){
  if ( (ModoCO2 == 1) && ((TempoTranscorrido - UltimoAjusteCO2) >= dtCO2) ) {
    lerGas();
    if ( (CO2 < SetCO2) && (CO2 > 0) ) {
      ValveDelay = (SetCO2 - CO2)*0.4 + 10;                          // conferir estes parâmetros
      if (ValveDelay > MaxValveDelay) ValveDelay = MaxValveDelay;
      CO2On();                                                 
      delay(ValveDelay);
      CO2Off();
      CO2count += ValveDelay; 
    }
    UltimoAjusteCO2 = TempoTranscorrido;
  }
}

void imprimirGas(){
  if (ID == 4){ 
    Serial.print(ModoCO2); Serial.print(" ");
    Serial.print(GasTemp); Serial.print(" ");
    Serial.print(Umidade); Serial.print(" ");
    Serial.print(CO2); Serial.print(" ");
    Serial.print(SetCO2); Serial.print(" ");
    Serial.print(CO2count); Serial.print(" ");
  }
  else {
    for (byte i = 1; i <=6; i++) {
      Serial.print("n/a ");    
    }
  }
}

void CabecalhoGas(){
  Serial.print("ModoCO2"); Serial.print(" ");
  Serial.print("GasTemp ");
  Serial.print("Umidade ");
  Serial.print("CO2 ");
  Serial.print("SetCO2 ");
  Serial.print("CO2count ");
}



