
int IntervaloRampa = 200;

void ajustarAtuadores(){
  ajustarPainel();
  ajustarMotores();
  ajustarPeltier(); 
}

void pararAtuadores(){
  for(byte i = 0; i <= 10; i++){
    for(byte j = 0; j<16; j++){
      int pwm = map(Valor[j]*(1-0.1*i),0,100,0,4095);
      ServoDriver.setPWM(Canal[j],0,pwm);
    }
    delay(IntervaloRampa);
  }
  for(byte i = 0; i<16; i++){
    ServoDriver.setPWM(Canal[i],0,0);  
  }
}

void ligarAtuadores(){
  for(byte i = 0; i <= 10; i++){
    for(byte j = 0; j<16; j++){
      int pwm = map(Valor[j]*(0.1*i),0,100,0,4095);
      ServoDriver.setPWM(Canal[j],0,pwm);
    }
    delay(IntervaloRampa);
  }
}

void imprimirDados(){
  Serial.print(ID); Serial.print(" ");
  imprimirTempo();
  imprimirMotores();
  imprimirPainel();
  imprimirPeltier();
  imprimirDensidade();
  imprimirDiluicao();
  imprimirPH();
  imprimirGas();
  Serial.println();
}

void lerSensores(){
  medirTemperatura();
  lerPH();    // inserir a correção de temperatura
  medirDensidade();
  lerGas();
  if (ModoDeOperacao == 2) imprimirDados();
}

void imprimirColunas(){
  Serial.print("ID ");
  CabecalhoTempo();
  CabecalhoMotores();
  CabecalhoPainel();
  CabecalhoPeltier();
  CabecalhoDensidade();
  CabecalhoDiluicao();  
  CabecalhoPH();
  CabecalhoGas();
}

void imprimirCabecalho(){
  Serial.print("Spectrum, Versao "); Serial.println(Versao);
  Serial.print("Reator: "); Serial.println(ID);
  Serial.print("ID ");
  imprimirColunas();
  Serial.println();
}

void desligarTudo(){                // conferir: é preciso alterar os valores do compressor e da ventoinha?
  pararAtuadores();
  for(byte i = 0; i<16; i++){
    Valor[i] = 0;  
  }
  desligarLEDs();
  BrilhoMax = 0;
  ModoTemp = 0;
  ModoDiluicao = 0;
}

void adquirirDados(){
  pararAtuadores();
  lerSensores();
  ligarAtuadores();
  imprimirDados();
  diluir();
  UltimaImpressao = TempoTranscorrido;
}

void iniciarExperimento(){
  iniciarTempo();
  imprimirCabecalho();
  UltimaMudancaPainel = 0;
}

void curvaLEDs(){
  Serial.println();
  Serial.println("*** inicio da curva dos LEDs ***");
  Serial.print("Ganho: "); Serial.print(tsl.getGain());
  Serial.print(" Tempo: "); Serial.println((tsl.getTiming() + 1) * 100, DEC);//Serial.println();
  
  for(byte i = 0; i<=100; i=i+5){
  
    int pwm = map(i, 0, 100, 0, 255);
  
    analogWrite(RedPin, pwm);
    delay(LEDdelay);
    Vis_R = tsl.getLuminosity(TSL2591_VISIBLE);
    digitalWrite(RedPin, LOW);
  
    analogWrite(BluePin, pwm);
    delay(LEDdelay);
    Vis_B = tsl.getLuminosity(TSL2591_VISIBLE);
    digitalWrite(BluePin, LOW);
  
    analogWrite(IRPin, pwm);  
    delay(LEDdelay);
    Vis_IR = tsl.getLuminosity(TSL2591_VISIBLE);
    digitalWrite(IRPin, LOW); 

    Serial.print(i); Serial.print(" ");
    Serial.print(Vis_R); Serial.print(" ");
    Serial.print(Vis_B); Serial.print(" ");
    Serial.print(Vis_IR); Serial.print(" ");
    Serial.println();
  }
  
  Serial.println("*** fim da curva dos LEDs ***");
  Serial.println();
}

void alterarParametros(){     // esta funcao pode ser chamada via lvp para dar inicio a um experimento com parametros pre-definidos (comentar ou inserir os respectivos valores)
//  dt = 300;
  
  Valor[12] = 0;     // ar
  Valor[13] = 0;    // ima
  
  ModoCor = 0;
  ModoBrilho = 0;
  BrilhoAtual = 5;   // este será o valor inicial do brilho no modo step (esta variavel nao pode ser alterada via LVP)
  BrilhoMax = 0;
  IncrementoBrilho = 5;
//  BomDia = 6;
//  BoaNoite = 18;
  PeriodoPainel = 7200;

//  MaxManual[0] = 0;   // branco
//  MaxManual[1] = 0;   // full
//  MaxManual[2] = 0;   // 440
//  MaxManual[3] = 0;   // 470
//  MaxManual[4] = 0;   // 495
//  MaxManual[5] = 0;   // 530
//  MaxManual[6] = 0;   // 595
//  MaxManual[7] = 0;   // 634
//  MaxManual[8] = 0;   // 660
//  MaxManual[9] = 0;   // 684
  
  ModoTemp = 0;
  TempDesejada = 27;
  
  DensidadeDesejada = 50000000;
  ModoDiluicao = 0;
  
  ModoDeOperacao = 0;  
}

void ping(){
  /*Returns ID*/
  Serial.print(ID);
}
