
/*
 *  
 *  Pendências: 
 *  ok - Inserir função de ajuste do relógio
 *  ok - Inserir função de calibração do pH
 *  ok - Mesclar os programas algas e componentes (diferentes modos?)
 *  - Salvar versões corretas das bibliotecas utilizadas e especificar um caminho diferente
 *  - a leitura de ph esta compensando a temperatura?
 *  ok - inserir modo componentes 
 *  - Peltier: leitura de corrente
 *  - Calibrar densidade
 *  - Incluir gas
 *  - diluicao (remover funcoes do modo componentes?)
 *  ok - incluir m1 e m2 no lvp no modo componentes
 *  - definir como serão os modos da diluição
----------------------------------------
*/

byte Versao = 1;
byte ID = 7;

byte ModoDeOperacao = 0; // 0 para leituras apenas quando solicitadas; 1 para leituras automaticas dentro do dt; 2 para o modo "componentes"

/*
 * 
 *  Pinos em uso:
 *  
 *  A0  CS PonteH
 *  
 *  2   Termômetro
 *  
 *  8   Enable PonteH
 *  
 *  9   LED IR
 *  10  LED B
 *  11  LED R
 *  
 *  12  InA PonteH
 *  13  InB PonteH
 *  
 *  20  SDA servoDriver e sensor CO2
 *  21  SCL servoDriver e sensor CO2
 *  
 *  40 (saída 1)
 *  41  CO2 (saída 2)
 *  
 *  42  SCL relógio
 *  43  SDA relógio
 * 
 *  52  Rx Atlas
 *  53  Tx Atlas
 * 
 */
 
