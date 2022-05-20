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
byte medidas = 3;

uint16_t Vis_R, Vis_B, Vis_IR;

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

  if(tsl.begin())Serial.println("TSL ok");
  else Serial.println("ERRO no TSL ");
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
    Vis_R = 0;
    Vis_B = 0;
    Vis_IR = 0;
    
    for(byte i=1; i <= medidas; i++){
      pwm = map(RedPwm, 0, 100, 0, 255);
      analogWrite(RedPin, pwm);
      delay(LEDdelay);
      Vis_R += tsl.getLuminosity(TSL2591_VISIBLE);
      digitalWrite(RedPin, LOW);
      
      pwm = map(BluePwm, 0, 100, 0, 255);
      analogWrite(BluePin, pwm);
      delay(LEDdelay);
      Vis_B += tsl.getLuminosity(TSL2591_VISIBLE);
      digitalWrite(BluePin, LOW);
      
      pwm = map(IRPwm, 0, 100, 0, 255);
      analogWrite(IRPin, pwm);  
      delay(LEDdelay);
      Vis_IR += tsl.getLuminosity(TSL2591_VISIBLE);
      digitalWrite(IRPin, LOW);   
    }
    
    Vis_R /= medidas;
    Vis_B /= medidas;
    Vis_IR /= medidas;

    if (ID == 1) DensidadeAtual = (71.76272*exp(-1.0*Vis_IR/354.32271) + 8.00350*exp(-1.0*Vis_IR/5506.99432) - 0.62136)*1E6;
    if (ID == 2) DensidadeAtual = (68.10010*exp(-1.0*Vis_IR/341.86234) + 7.56218*exp(-1.0*Vis_IR/6009.17417) - 0.60030)*1E6;
    if (ID == 3) DensidadeAtual = (69.40844*exp(-1.0*Vis_IR/312.08378) + 7.85397*exp(-1.0*Vis_IR/4884.56583) - 0.54076)*1E6;
    if (ID == 4) DensidadeAtual = (68.81832*exp(-1.0*Vis_IR/332.10452) + 7.95428*exp(-1.0*Vis_IR/5353.53152) - 0.73229)*1E6;
    if (ID == 5) DensidadeAtual = (72.15072*exp(-1.0*Vis_IR/325.07721) + 7.47414*exp(-1.0*Vis_IR/4989.12444) - 0.30981)*1E6;
    if (ID == 6) DensidadeAtual = (68.10000*exp(-1.0*Vis_IR/292.42084) + 7.36802*exp(-1.0*Vis_IR/4406.68303) - 0.16793)*1E6;
    if (ID == 7) DensidadeAtual = (70.45496*exp(-1.0*Vis_IR/355.07195) + 7.83826*exp(-1.0*Vis_IR/5570.96923) - 0.49460)*1E6;
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
