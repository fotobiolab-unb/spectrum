
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver ServoDriver = Adafruit_PWMServoDriver(0x40);

byte ValorBranco = 0;
byte ValorFull = 0;
byte Valor684 = 0;
byte Valor660 = 0;
byte Valor634 = 0;
byte Valor595 = 0;
byte Valor530 = 0;
byte Valor495 = 0;
byte Valor470 = 0;
byte Valor440 = 0;
byte ValorSaida = 0;
byte ValorEntrada = 0;
byte ValorComp = 0;
byte ValorAgitador = 0;
byte ValorVentoinha = 0;
byte ValorPeltier = 0;

float Valor[] = {ValorBranco, ValorFull, Valor440, Valor470, Valor495, Valor530, Valor595, Valor634, Valor660, Valor684, ValorEntrada, ValorSaida, ValorComp, ValorAgitador, ValorVentoinha, ValorPeltier};
byte Canal[] = {14, 13, 1, 2, 3, 6, 8, 11, 15, 12, 5, 7, 4, 10, 9, 0};

void iniciarServo(){
  ServoDriver.begin();
  ServoDriver.setPWMFreq(1600);
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



// *************** Programa componentes: (isso é usado na diluicao)

byte Motor3pwm=0;
byte Motor4pwm=0;


