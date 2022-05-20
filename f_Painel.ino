
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

int limite[10];

void igualarLEDs(){
  if (ID == 1){
    limite[0] = 3886;
    limite[1] = 3889;
    limite[2] = 3266;
    limite[3] = 3623;
    limite[4] = 3630;
    limite[5] = 3917;
    limite[6] = 3914;
    limite[7] = 3203;
    limite[8] = 3497;
    limite[9] = 3939;
  }
  if (ID == 2){
    limite[0] = 4088;
    limite[1] = 3968;
    limite[2] = 4048;
    limite[3] = 3879;
    limite[4] = 3619;
    limite[5] = 3849;
    limite[6] = 3900;
    limite[7] = 3093;
    limite[8] = 3532;
    limite[9] = 3842;
  }
  if (ID == 3){
    limite[0] = 3612;
    limite[1] = 4043;
    limite[2] = 4028;
    limite[3] = 4001;
    limite[4] = 3821;
    limite[5] = 4017;
    limite[6] = 3741;
    limite[7] = 3345;
    limite[8] = 3558;
    limite[9] = 4084;
  }
  if (ID == 4){
    limite[0] = 3753;
    limite[1] = 3967;
    limite[2] = 4009;
    limite[3] = 4080;
    limite[4] = 4079;
    limite[5] = 4085;
    limite[6] = 4082;
    limite[7] = 4080;
    limite[8] = 4091;
    limite[9] = 3953;
  }
  if (ID == 5){
    limite[0] = 3728;
    limite[1] = 4033;
    limite[2] = 3747;
    limite[3] = 3877;
    limite[4] = 3516;
    limite[5] = 3876;
    limite[6] = 3647;
    limite[7] = 3352;
    limite[8] = 3248;
    limite[9] = 3969;
  }
  if (ID == 6){
    limite[0] = 3644;
    limite[1] = 3809;
    limite[2] = 3603;
    limite[3] = 4021;
    limite[4] = 3513;
    limite[5] = 3847;
    limite[6] = 3703;
    limite[7] = 3283;
    limite[8] = 3663;
    limite[9] = 3757;
  }
  if (ID == 7){
    limite[0] = 3565;
    limite[1] = 4091;
    limite[2] = 3716;
    limite[3] = 3794;
    limite[4] = 3405;
    limite[5] = 3695;
    limite[6] = 3672;
    limite[7] = 3117;
    limite[8] = 3792;
    limite[9] = 3970;
  }
}

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
    pwm = map(Valor[i],0,100,0,limite[i]);
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
