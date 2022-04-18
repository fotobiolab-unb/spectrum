
float MaxManual[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float MaxSol[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};               //  inserir valores após a obtenção 
float MaxChlor[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};             //  dos espectros
float MaxNeutro[] = {36, 100, 24, 34, 45, 43, 25, 17, 19, 36};
float MaximoPainel[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float BrilhoMax = 0;
float BrilhoAtual = 0;

byte ModoCor = 0;      // 0: manual; 1: sol; 2: clorofila; 3: neutro
byte ModoBrilho = 0;   // 0: fixo; 1: dia/noite; 2: senoidal (com a hora); 3: ligado/desligado; 4: step

byte BomDia = 6;      // Variáveis usadas apenas no modo 2, alteráveis apenas via LVP, 
byte BoaNoite = 18;   // representam a hora do amanhecer e anoitecer

unsigned long UltimaMudancaPainel = 0;
long PeriodoPainel = 43200; // (em segundos)
float IncrementoBrilho = 0; // tamanho do degrau no modo step
boolean EstadoPainel = true;

void ajustarBrilho(){
  if (ModoBrilho == 0) {
    BrilhoAtual = BrilhoMax;
  }
  else if (ModoBrilho == 1) {                                                             
    if ( Hora >= BomDia && Hora < BoaNoite ) BrilhoAtual = BrilhoMax;
    else BrilhoAtual = 0;  
  }
  else if (ModoBrilho == 2){
    BrilhoAtual = BrilhoMax/2 * ( 1 - cos( (2*PI*( Hora*60 + Minuto)) / 1440 ) );
  }
  else if (ModoBrilho == 3){
    if ( (millis() - UltimaMudancaPainel) >= (PeriodoPainel) ) {
      if (!EstadoPainel) EstadoPainel = true;
      else if (EstadoPainel) EstadoPainel = false;
      UltimaMudancaPainel = millis();      
    }
    BrilhoAtual = EstadoPainel * BrilhoMax;    
  }
  else if (ModoBrilho == 4){
    if ( (TempoTranscorrido-UltimaMudancaPainel) >= PeriodoPainel ) {
      BrilhoAtual = BrilhoAtual + IncrementoBrilho; 
      if ( (BrilhoAtual > BrilhoMax) ) {        
        IncrementoBrilho = -IncrementoBrilho;      
        BrilhoAtual = BrilhoAtual + 2*IncrementoBrilho;  // 1*PanelStep ou 2*PanelStep?   
      }
      if ( (BrilhoAtual < 0) ) {   
        IncrementoBrilho = -IncrementoBrilho;
        BrilhoAtual = BrilhoAtual + 2*IncrementoBrilho;  // 1*PanelStep ou 2*PanelStep? 
      }
                                                   
      UltimaMudancaPainel = TempoTranscorrido;
    }
  }
}

void ajustarCor(){
  if (ModoCor == 0){
    for(byte i = 0; i <= 9; i++){
      MaximoPainel[i] = MaxManual[i];
    }
  }
  else if (ModoCor == 1){
    for(byte i = 0; i <= 9; i++){
      MaximoPainel[i] = MaxSol[i];
    }    
  }
  else if (ModoCor == 2){
    for(byte i = 0; i <= 9; i++){
      MaximoPainel[i] = MaxChlor[i];
    }    
  }
  else if (ModoCor == 3){
    for(byte i = 0; i <= 9; i++){
      MaximoPainel[i] = MaxNeutro[i];
    }   
  }
}

void ajustarPainel(){
  ajustarBrilho();
  ajustarCor();
  
  int pwm;
  for (byte i = 0; i <=9; i++){
    Valor[i] = MaximoPainel[i]*BrilhoAtual/100;
    pwm = map(Valor[i],0,100,0,4095);
    ServoDriver.setPWM(Canal[i],0,pwm); 
  }
}

void desligarPainel(){
  for (byte i = 0; i<9; i++){
    ServoDriver.setPWM(Canal[i],0,0);  
  }
}

void imprimirPainel(){
  Serial.print(BrilhoAtual,3); Serial.print(" ");
  Serial.print(ModoCor); Serial.print(" ");
  Serial.print(ModoBrilho); Serial.print(" ");
  for (byte i = 0; i <= 9; i++){
    Serial.print(Valor[i],3); Serial.print(" ");
  }
}

void CabecalhoPainel(){
  Serial.print("Brilho ");
  Serial.print("Cor ");
  Serial.print("Modo ");
  Serial.print("Branco Full 440 470 495 530 595 634 660 684 ");
}
