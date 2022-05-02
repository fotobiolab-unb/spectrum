
// RTC em uso para a obtenção do horário impresso com os dados e para o controle de iluminação
// Contagem interna do Arduino em uso para a conferência do ElapsedTime
   
#include <Time.h>
#include <DS1307.h>

const byte Clock_SDA_Pin = 43;
const byte Clock_SCL_Pin = 42;

DS1307 RTC(Clock_SDA_Pin, Clock_SCL_Pin);

byte Hora;     // variável usada para o controle da iluminação baseado no horário
byte Minuto;   // variável usada para o controle da iluminação baseado no horário
unsigned long TempoTranscorrido;
unsigned long TempoTranscorrido_0 = 0;
unsigned long UltimaImpressao = 0;
int dt = 300;

byte Hora0, Minuto0, Dia0, Mes0;        //
int Ano0;                               // variáveis usadas no ajuste do relógio (via LVP apenas)

void verificarHora(){                                             //
  String ThisMoment;                                              //    Esta função é chamada a cada loop para
  String HourString;                                              //    permitir o ajuste da iluminação  
  String MinuteString;                                            //    
  TempoTranscorrido = millis()/1000L - TempoTranscorrido_0;       //   
  ThisMoment = RTC.getTimeStr();                                  //
  HourString = ThisMoment.substring(0,2);                         //
  Hora = HourString.toInt();                                      //
  MinuteString = ThisMoment.substring(3,5);                       //
  Minuto = MinuteString.toInt();                                  //
}                                                                 //

void iniciarTempo(){                          //
  RTC.halt(false);                            // Esta função é executada no setup
  TempoTranscorrido_0 = millis()/1000L;       // e ao iniciar um novo experimento
  UltimaImpressao = 0;                        //
}                                             //

void imprimirTempo(){
  Serial.print(TempoTranscorrido); Serial.print(" ");
  Serial.print(RTC.getDateStr()); Serial.print(" "); 
  Serial.print(RTC.getTimeStr()); Serial.print(" "); 
}

void CabecalhoTempo(){
  Serial.print("TempoTranscorrido ");
  Serial.print("Data ");
  Serial.print("Hora ");
}

void ajustarRelogio(){              // Esta função pode ser chamada (via LVP) caso o relógio necessite de ajuste
  //rtc.setDOW(TUESDAY);            // Define o dia da semana
  RTC.setTime(Hora0, Minuto0, 0);   // Define o horario (hh, mm, ss)
  RTC.setDate(Dia0, Mes0, Ano0);    // Define o dia, mes e ano (dd, mm, aaaa)
}

