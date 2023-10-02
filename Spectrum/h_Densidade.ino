float DensidadeAtual;

// **************************************************** leds *********************************************
const byte RedPin = 11;
const byte BluePin = 10;
const byte IRPin = 9;

byte RedPwm = 100;
byte BluePwm = 100;
byte IRPwm = 100;

int LEDdelay = 1000;

void ajustarLEDs(){                     //
  byte pwm;                             //
  pwm = map(RedPwm, 0, 100, 0, 255);    //
  analogWrite(RedPin, pwm);             //    Função usada para ajustar os LEDs
  pwm = map(BluePwm, 0, 100, 0, 255);   //    quando o reator estiver funcionando
  analogWrite(BluePin, pwm);            //    no modo "componentes"
  pwm = map(IRPwm, 0, 100, 0, 255);     //
  analogWrite(IRPin, pwm);              //
}                                       //

void desligarLEDs(){
  RedPwm = 0;
  BluePwm = 0;
  IRPwm = 0;
  ajustarLEDs();
}

// **************************************************** TSL *********************************************
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

byte ganhoTSL = 1;
byte itTSL = 6;

int Vis_R, Vis_B, Vis_IR;

void configurarSensor() {
  switch (ganhoTSL) {
    case 1:
      tsl.setGain(TSL2591_GAIN_LOW);
      break;
    case 2:
      tsl.setGain(TSL2591_GAIN_MED);
      break;
    case 3:
      tsl.setGain(TSL2591_GAIN_HIGH);
      break;
  }

  switch (itTSL) {
    case 1:
      tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);
      break;
    case 2:
      tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
      break;
    case 3:
      tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
      break;
    case 4:
      tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
      break;
    case 5:
      tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
      break;
    case 6:
      tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);
      break;
  }

  Serial.print("Ganho: "); 
  Serial.print(tsl.getGain());
  Serial.print(" Tempo: ");
  Serial.print((tsl.getTiming() + 1) * 100, DEC);
  Serial.println();
}

// **************************************************** Funcoes *********************************************
void iniciarSensor(){
  pinMode(RedPin, OUTPUT);
  pinMode(BluePin, OUTPUT);
  pinMode(IRPin, OUTPUT);
  digitalWrite(RedPin, LOW);
  digitalWrite(BluePin, LOW);
  digitalWrite(IRPin, LOW);  

  /*
  if(tsl.begin())Serial.println("TSL ok");
  else Serial.println("ERRO no TSL ");
  */
  configurarSensor();
}

void medirDensidade(){

  if (ModoDeOperacao == 2){
    Vis_R = tsl.getLuminosity(TSL2591_VISIBLE);
    Vis_B = tsl.getLuminosity(TSL2591_VISIBLE);
    Vis_IR = tsl.getLuminosity(TSL2591_VISIBLE);
    DensidadeAtual = 0;
  }

  else {
    byte pwm;
    
    pwm = map(RedPwm, 0, 100, 0, 255);
    analogWrite(RedPin, pwm);
    delay(LEDdelay);
    Vis_R = tsl.getLuminosity(TSL2591_VISIBLE);
    //IR_R = tsl.getLuminosity(TSL2591_INFRARED);
    digitalWrite(RedPin, LOW);
    
    pwm = map(BluePwm, 0, 100, 0, 255);
    analogWrite(BluePin, pwm);
    delay(LEDdelay);
    Vis_B = tsl.getLuminosity(TSL2591_VISIBLE);
    //IR_B = tsl.getLuminosity(TSL2591_INFRARED);
    digitalWrite(BluePin, LOW);
    
    pwm = map(IRPwm, 0, 100, 0, 255);
    analogWrite(IRPin, pwm);  
    delay(LEDdelay);
    Vis_IR = tsl.getLuminosity(TSL2591_VISIBLE);
    //IR_IR = tsl.getLuminosity(TSL2591_INFRARED);
    digitalWrite(IRPin, LOW); 

/*#*/DensidadeAtual = (70.45496*exp(-Vis_IR/355.07195) + 7.83826*exp(-Vis_IR/5570.96923) - 0.49460)*1E6;
 
  }
}

void imprimirDensidade(){
  Serial.print(RedPwm); Serial.print(" ");
  Serial.print(BluePwm); Serial.print(" ");
  Serial.print(IRPwm); Serial.print(" ");
  Serial.print(Vis_R); Serial.print(" ");
  Serial.print(Vis_B); Serial.print(" ");
  Serial.print(Vis_IR); Serial.print(" ");
  Serial.print(DensidadeAtual); Serial.print(" ");
}

void CabecalhoDensidade(){
  Serial.print("RedPwm ");
  Serial.print("BluePwm ");
  Serial.print("IRPwm ");
  Serial.print("Vis_R ");
  Serial.print("Vis_B ");
  Serial.print("Vis_IR ");
  Serial.print("DensidadeAtual ");
}


