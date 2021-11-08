// ************ Termometro ************

#include<DS18S20.h>

const byte TermoPin = 2;

DS18S20 termo(TermoPin);

float Temp;
float TempDesejada = 25; // ************ 

void medirTemperatura(){
  Temp = termo.getTemp();
}


// ************ Ponte H ************

const byte InA = 12;
const byte InB = 13;
const byte CSPin = A0;
const byte NumeroMedidas = 50;
const byte EnablePin = 8;

String EstadoPeltier;
float Corrente = 0;
boolean SentidoPelt = false;

byte Cp = 100; 
float Ci = 0.003;
float Cd = 0;
float Int = 0;

byte ModoTemp = 0;  // 0: controle desligado; 1: temp = const; 2: controle manual do peltier
int ResfMax = 100;
int AqMax = 50;
int PeltPWM = 0;
float UltimaTemp = 0;

void aquecer(){
  digitalWrite(EnablePin, HIGH);
  digitalWrite(InA, HIGH);
  digitalWrite(InB, LOW);  
  EstadoPeltier = "aquecendo"; 
  SentidoPelt = false;
}

void resfriar(){
  digitalWrite(EnablePin, HIGH);
  digitalWrite(InA, LOW);
  digitalWrite(InB, HIGH);
  EstadoPeltier = "resfriando";
  SentidoPelt = true;
}

void inverterPelt(){            //
  if (SentidoPelt) aquecer();   //  Modo componentes
  else resfriar();              //
}                               //

void desligarPeltier(){
  digitalWrite(EnablePin, LOW);
  digitalWrite(InA, LOW);
  digitalWrite(InB, LOW);
  EstadoPeltier = "desligado";  
  Valor[15] = 0;
  Valor[14] = 0;   
}

void ajustarPeltier(){
  if ( ModoTemp == 0 ) desligarPeltier();
  else if ( ModoTemp == 1 ) {
    medirTemperatura();
    float ErroTemp = Temp - TempDesejada;
    if (abs(ErroTemp) > 0.1) {
      
      Int += ErroTemp * Ci;
      if (Int > ResfMax) Int = ResfMax;
      else if (Int < -AqMax) Int = -AqMax;

      float dTemp = Temp - UltimaTemp;

      PeltPWM = ErroTemp * Cp + Int - dTemp * Cd;
      
      if (PeltPWM > 0){
        resfriar();
        if (PeltPWM > ResfMax) PeltPWM = ResfMax;
      }
      else if (PeltPWM < 0){
        aquecer();
        PeltPWM = abs(PeltPWM);
        if (PeltPWM > AqMax) PeltPWM = AqMax;
      }
    UltimaTemp = Temp;  
    }
    Valor[15] = PeltPWM;
    if (Temp == -1000){   // erro de leitura (termometro desconectado)
      Valor[15] = 0; 
      EstadoPeltier = "erro";
    }
    if (Valor[15] != 0) Valor[14] = 100;
  }

  for(byte i = 14; i<16; i++){
    int pwm = map(Valor[i],0,100,0,4095);
    ServoDriver.setPWM(Canal[i],0,pwm);
  }
}

void imprimirPeltier(){
  Serial.print(Temp); Serial.print(" ");
  Serial.print(TempDesejada); Serial.print(" ");
  Serial.print(Valor[15]); Serial.print(" ");
  Serial.print(Int); Serial.print(" ");
  Serial.print(ModoTemp); Serial.print(" ");
  Serial.print(Cp); Serial.print(" ");
  Serial.print(Ci,3); Serial.print(" ");
  Serial.print(Cd,1); Serial.print(" ");
  Serial.print(EstadoPeltier); Serial.print(" ");
}

void CabecalhoPeltier(){
  Serial.print("Temp ");
  Serial.print("SetTemp ");
  Serial.print("ValorPeltier ");
  Serial.print("Int ");
  Serial.print("ModoTemp ");
  Serial.print("Cp ");
  Serial.print("Ci ");
  Serial.print("Cd ");
  Serial.print("EstadoPeltier ");  
}

void iniciarPeltier(){
  pinMode(InA, OUTPUT);
  pinMode(InB, OUTPUT);
  pinMode(EnablePin, OUTPUT);
  pinMode(CSPin,INPUT);
  desligarPeltier();
}

float obterCorrente() {
/*  float leitura=0;
  for (int i=0;i<NumeroMedidas;i++){
    leitura += analogRead(CSPin);
    delay(1);
  }
  //Corrente = (((leitura/NumeroMedidas)*5.0/1024.0)/0.13);
  Corrente = leitura/NumeroMedidas/0.13;
  */
  Corrente = analogRead(CSPin);
}
