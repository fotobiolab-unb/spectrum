float DensidadeDesejada = 999999999; // ***********************

byte ModoDiluicao = 0; // 0: densidade livre; 1: densidade constante
double DiluicaoMaxima = 30000; // (em ms) ****************** escolher um valor adequado (e atentar para o tipo de variavel) **************
long DiluicaoTotal = 0;
byte ValorDiluicao = 60; // ******** escolher ********
byte DiluicaoManual = 0; // em segundos
double TempoDiluicao; // em ms
int pwmDil;

void adicionarMeio(long tempo){
  ServoDriver.setPWM(Canal[10],0,pwmDil);
  delay(tempo);
  ServoDriver.setPWM(Canal[10],0,0);
}

void removerAlgas(long tempo){
  ServoDriver.setPWM(Canal[11],0,pwmDil);
  delay(tempo);
  ServoDriver.setPWM(Canal[11],0,0);
}

void diluir(){
  // IRDesejado = // ajustar f(DensidadeDesejada)
  if ( (ModoDiluicao == 1) && (DensidadeAtual > DensidadeDesejada) ){
    pwmDil = map(ValorDiluicao,0,100,0,4095);
    //TempoDiluicao = (DensidadeAtual/DensidadeDesejada - 1) * 310000;             // VolumeDoReator/VazaoDaBombaDeDiluicao = 100mL/(10mL/31s)
    TempoDiluicao = (DensidadeAtual/DensidadeDesejada - 1) * 320000;
    if (TempoDiluicao > DiluicaoMaxima) TempoDiluicao = DiluicaoMaxima;
    
    adicionarMeio(TempoDiluicao); 
    delay(2000);
    ServoDriver.setPWM(Canal[12],0,0);
    delay(2000);
    removerAlgas(1.5*TempoDiluicao); 
    
    DiluicaoTotal += TempoDiluicao;    
  }
}

void drenar(){
  pwmDil = map(ValorDiluicao,0,100,0,4095);
  removerAlgas(5000);
}

void imprimirDiluicao(){
  Serial.print(DensidadeDesejada); Serial.print(" ");
  Serial.print(ValorDiluicao); Serial.print(" ");
  Serial.print(ModoDiluicao); Serial.print(" ");
  Serial.print(DiluicaoTotal); Serial.print(" ");
}

void CabecalhoDiluicao(){
  Serial.print("DensidadeDesejada ");
  Serial.print("ValorDiluicao ");
  Serial.print("ModoDiluicao ");
  Serial.print("DiluicaoTotal ");  
}

void diluirManualmente(){
  TempoDiluicao = DiluicaoManual*1000;
  pwmDil = map(ValorDiluicao,0,100,0,4095);
  if (TempoDiluicao > DiluicaoMaxima) TempoDiluicao = DiluicaoMaxima;
  adicionarMeio(TempoDiluicao); 
  delay(3000);
  ServoDriver.setPWM(Canal[12],0,0);
  removerAlgas(2*TempoDiluicao); 
  DiluicaoTotal += TempoDiluicao; 
}

