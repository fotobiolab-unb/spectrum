// ****************************** Sensor pH *******************************
#include <AtlasPH_Spectrum.h>
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
  Serial.println("-> Calibracao do sensor de pH iniciada.");
  Serial.println("-> Insira a sonda na solucao de referencia pH 7. Quando as leituras se estabilizarem digite 'q' e pressione 'enter'.");
  Serial.println("-> (Para interromper a calibração pressione qualquer outra tecla)");  
  aguardarComando();
  if (inputString == "q") {
    Serial.println("-> cal,mid,7");
    SondaPH.calPH7();
    Serial.println("-> Insira a sonda na solucao de referencia pH 4. Quando as leituras se estabilizarem digite 'q' e pressione 'enter'.");
    Serial.println("-> (Para interromper a calibração pressione qualquer outra tecla)");  
    aguardarComando();
    if (inputString == "q") {
      Serial.println("-> cal,low,4");
      SondaPH.calPH4();
      Serial.println("-> Insira a sonda na solucao de referencia pH 10. Quando as leituras se estabilizarem digite 'q' e pressione 'enter'.");
      Serial.println("-> (Para interromper a calibração pressione qualquer outra tecla)");  
      aguardarComando();
      if (inputString == "q") {
        Serial.println("-> cal,high,10");
        SondaPH.calPH10();
      }  
    }
    Serial.println(SondaPH.getPHValue()); delay(1000);  
    Serial.println(SondaPH.getPHValue()); delay(1000);
    Serial.println(SondaPH.getPHValue()); delay(1000);
  }
  Serial.println("-> Calibracao concluida");
}
