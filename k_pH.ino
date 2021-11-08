// ****************************** Sensor pH *******************************
#include <AtlasPH_ReatorNovo.h>
#include <SoftwareSerial.h>

const byte RXPin = 52;
const byte TXPin = 53;

AtlasPH SondaPH(RXPin, TXPin);
SoftwareSerial sSerial(RXPin,TXPin);

float pH;

void lerPH(){
  pH = SondaPH.getPHValue(Temp);
}

void imprimirPH(){
  Serial.print(pH); Serial.print(" ");  
}

void CabecalhoPH(){
  Serial.print("pH ");
}

void iniciarPH(){
  SondaPH.initialize();
}

// *************************** calibracao *****************************

boolean  DataReceived = false;
char inChar;
char response;

String inputString = "";

void aguardarComando(){
  DataReceived = false;
  inputString = "";
  
  while ( !DataReceived ){
    Serial.println(SondaPH.getPHValue());
    delay(1000);
    if ( Serial.available() > 0) {
      inChar = (char) Serial.read();
      if ( (inChar != '\n') && (inChar != '\r') ) inputString += inChar;
      else DataReceived = true;
    }
  }
  delay(50);
  while (Serial.available() > 0) { Serial.read();}  
}



void calibrarPH(){

  String inputstring = "";                              //a string to hold incoming data from the PC
  String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
  boolean input_string_complete = false;                //have we received all the data from the PC
  boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product

  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product 

  Serial.println("-> Calibracao do sensor de pH iniciada.");
  
  Serial.println("-> Insira a sonda na solucao de referencia pH 7. Quando as leituras se estabilizarem digite 'q' e pressione 'enter'.");
  aguardarComando();
  if (inputString == "q") {
    Serial.println("-> cal,mid,7");
    
        sSerial.print("c,1");
        sSerial.print('\r'); 
        sSerial.print("l,1");
        sSerial.print('\r'); 
 //   sSerial.print("cal,mid,7");
 //   sSerial.print('\r'); 
  }

  Serial.println("-> Insira a sonda na solucao de referencia pH 4. Quando as leituras se estabilizarem digite 'q' e pressione 'enter'.");
  aguardarComando();
  if (inputString == "q") {
sSerial.print("c,0");
    sSerial.print('\r'); 

    sSerial.print("l,0");
    sSerial.print('\r'); 
    
    Serial.println("-> cal,mid,4");
    sSerial.print("cal,mid,4");
    sSerial.print('\r'); 
  }

  Serial.println("-> Insira a sonda na solucao de referencia pH 10. Quando as leituras se estabilizarem digite 'q' e pressione 'enter'.");
  aguardarComando();
  if (inputString == "q") {
    Serial.println("-> cal,mid,10");
    sSerial.print("cal,mid,10");
    sSerial.print('\r'); 
  }

  Serial.println(SondaPH.getPHValue()); delay(1000);  
  Serial.println(SondaPH.getPHValue()); delay(1000);
  Serial.println(SondaPH.getPHValue()); delay(1000);
  Serial.println("-> Calibracao concluida");
  
}

