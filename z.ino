void setup(){
  iniciarLVP();
  iniciarTempo();
  iniciarServo();
  iniciarPeltier();
  iniciarSensor();
  iniciarPH();
  iniciarGas();
  imprimirCabecalho();
}

void loop(){
  ajustarAtuadores();
  if (ID == 4) ajustarCO2();
 
  if (ModoDeOperacao == 0){                           // leituras apenas quando solicitadas
    verificarHora();   
  }

  else if (ModoDeOperacao == 1){                      // leituras automaticas dentro do dt
    verificarHora();
    if ( (TempoTranscorrido - UltimaImpressao) >= dt ) {
      adquirirDados();
    }
  }

  else if (ModoDeOperacao == 2){                      // modo componentes
    ModoBrilho = 0;
    ModoTemp = 2;
    ajustarLEDs(); 
  }
  delay(1000);
}
