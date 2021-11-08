void setup(){
  delay(10000);
  iniciarLVP();
  iniciarTempo();
  iniciarServo();
  iniciarPeltier();
  iniciarSensor();
  iniciarPH();
  imprimirCabecalho();
}

void loop(){
  ajustarAtuadores();
 
  if (ModoDeOperacao == 0){                         // leituras apenas quando solicitadas
    verificarHora();   
  }

  else if (ModoDeOperacao == 1){                      // leituras automaticas dentro do dt
    verificarHora();
    if ( (TempoTranscorrido - UltimaImpressao) >= dt ) adquirirDados();
  }

  else if (ModoDeOperacao == 2){                      // modo componentes
    ModoBrilho = 0;
    ModoTemp = 2;
    ajustarLEDs(); 
    if(diluindo && (TempoTranscorrido - InicioDil >= dilDt)) pararDiluicao();
  }
}

