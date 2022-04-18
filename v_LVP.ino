// **************************************************** LVP *********************************************
#include <LVP.h>

const word baudRate = 9600;
const byte numEditableData = 60;
const byte numFunctions = 25;

LVP lvp(numEditableData,numFunctions,baudRate);

void iniciarLVP(){
  lvp.initialize(); 

// Tempo 6+1
  lvp.addParameter("dt", &dt);            // Relevante apenas quando ModoDeOperacao = 2     
  lvp.addParameter("hora", &Hora0);               // ***
  lvp.addParameter("minuto", &Minuto0);           //  *
  lvp.addParameter("dia", &Dia0);                 //  * Ajuste do relógio
  lvp.addParameter("mes", &Mes0);                 //  *
  lvp.addParameter("ano", &Ano0);                 //  *
  lvp.addFunction("horacerta", &ajustarRelogio);  // ***

// Motores 2+0
  lvp.addParameter("ar", &Valor[12]);
  lvp.addParameter("ima", &Valor[13]);

// Painel 17+0
  lvp.addParameter("cor", &ModoCor);
  lvp.addParameter("modopainel", &ModoBrilho);
  lvp.addParameter("brilho", &BrilhoMax);
  lvp.addParameter("bomdia", &BomDia);
  lvp.addParameter("boanoite", &BoaNoite);  
  lvp.addParameter("tau", &PeriodoPainel);
  lvp.addParameter("step", &IncrementoBrilho);
  
  lvp.addParameter("branco", &MaxManual[0]);
  lvp.addParameter("full", &MaxManual[1]);
  lvp.addParameter("440", &MaxManual[2]);
  lvp.addParameter("470", &MaxManual[3]);
  lvp.addParameter("495", &MaxManual[4]);
  lvp.addParameter("530", &MaxManual[5]);
  lvp.addParameter("595", &MaxManual[6]);
  lvp.addParameter("634", &MaxManual[7]);
  lvp.addParameter("660", &MaxManual[8]);
  lvp.addParameter("684", &MaxManual[9]);

// Temperatura 4+1
  lvp.addParameter("modotemp", &ModoTemp);
  lvp.addParameter("temp", &TempDesejada);
  lvp.addParameter("vent", &Valor[14], Valor[14]); // componentes
  lvp.addParameter("pelt", &Valor[15], Valor[15]); // componentes
  lvp.addFunction("inv", &inverterPelt);           // componentes

// Densidade 9+3
  lvp.addParameter("r", &RedPwm);
  lvp.addParameter("b", &BluePwm);
  lvp.addParameter("ir", &IRPwm);
  lvp.addParameter("densidade", &DensidadeDesejada);
  lvp.addParameter("mododil", &ModoDiluicao);
  lvp.addParameter("dil", &ValorDiluicao);              // ***
  lvp.addParameter("dildt", &DiluicaoManual);
  lvp.addFunction("diluir", &diluirManualmente);
  lvp.addFunction("drenar", &drenar); 
  lvp.addFunction("gotod", &gotoDensity);
  lvp.addParameter("in", &Valor[10]);   // componentes
  lvp.addParameter("out", &Valor[11]);  // componentes

//Geral 1+8
  lvp.addFunction("fim", &desligarTudo);  
  lvp.addFunction("iniciar", &iniciarExperimento);
  lvp.addFunction("curva", &curvaLEDs);
  lvp.addFunction("dados", &adquirirDados);
  lvp.addParameter("modo", &ModoDeOperacao);
  lvp.addFunction("config", &alterarParametros);
  lvp.addFunction("ping", &ping);
  lvp.addFunction("cabecalho",&imprimirColunas);
  lvp.addFunction("dados", lerSensores);

//Sensor 2+1
  lvp.addParameter("ganho", &ganhoTSL, ganhoTSL);
  lvp.addParameter("ti", &itTSL, itTSL);
  lvp.addFunction("sensor", configurarSensor);

  lvp.addFunction("calph", calibrarPH);

// CO2 3+3  
  lvp.addParameter("modoco2", &ModoCO2);
  lvp.addParameter("co2", &SetCO2);
  lvp.addFunction("co2on", CO2On);
  lvp.addFunction("co2off", CO2Off);
  lvp.addParameter("co2ref", &CO2Ref);      // valor usado na calibracao do co2
  lvp.addFunction("calco2", calibrarCO2);

} 

void serialEvent(){ 
  lvp.getNewCommand();
} 
