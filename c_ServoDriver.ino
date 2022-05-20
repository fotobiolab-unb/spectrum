
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver ServoDriver = Adafruit_PWMServoDriver(0x40);

int ValorBranco = 0;
int ValorFull = 0;
int Valor684 = 0;
int Valor660 = 0;
int Valor634 = 0;
int Valor595 = 0;
int Valor530 = 0;
int Valor495 = 0;
int Valor470 = 0;
int Valor440 = 0;
int ValorSaida = 0;
int ValorEntrada = 0;
int ValorComp = 0;
int ValorAgitador = 0;
int ValorVentoinha = 0;
int ValorPeltier = 0;

float Valor[] = {ValorBranco, ValorFull, Valor440, Valor470, Valor495, Valor530, Valor595, Valor634, Valor660, Valor684, ValorEntrada, ValorSaida, ValorComp, ValorAgitador, ValorVentoinha, ValorPeltier};
byte Canal[] = {14, 13, 1, 2, 3, 6, 8, 11, 15, 12, 5, 7, 4, 10, 9, 0};

void iniciarServo(){
  ServoDriver.begin();
  ServoDriver.setPWMFreq(500);
  for(byte i = 0; i<16; i++){
    ServoDriver.setPWM(Canal[i],0,0);
  }
  igualarLEDs();
}

void ajustarMotores(){
  if (ModoDeOperacao == 2) {                  // No modo componentes é possível controlar os motores de diluição
    for(byte i = 10; i<14; i++){              
      int pwm = map(Valor[i],0,100,0,4095);   
      ServoDriver.setPWM(Canal[i],0,pwm);
    }    
  }
  else {                                      // Nos demais esta função ajusta apenas o compressor de ar e o agitador magnético
    for(byte i = 12; i<14; i++){
      int pwm = map(Valor[i],0,100,0,4095);
      ServoDriver.setPWM(Canal[i],0,pwm);
    }
  }
}

void imprimirMotores(){
  Serial.print(Valor[12]); Serial.print(" ");
  Serial.print(Valor[13]); Serial.print(" ");
  Serial.print(Valor[14]); Serial.print(" ");
}

void CabecalhoMotores(){
  Serial.print("Compressor ");
  Serial.print("Agitador ");
  Serial.print("Ventoinha ");
}
