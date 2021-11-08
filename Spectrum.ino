/*
 *  
 *  Alterações: 
 *  - Inserir função de ajuste do relógio
 *  - Inserir função de calibração do pH
 *  - Mesclar os programas algas e componentes (diferentes modos?)
 *  - Salvar versões corretas das bibliotecas utilizadas e especificar um caminho diferente
 *  - a leitura de ph esta compensando a temperatura?
 * 
 * - inserir modo componentes * 
 * 
 *  Peltier: leitura de corrente
 *  
 *  
----------------------------------------
elapsed time
data
hora
id

temp
ph
co2
densidade

LEDs (x10, 0-100)
brilho
cor (string?)
modoBrilho

setTemp
pwmPelt
estado
modo

pwmLEDs (x3)
luminosidade (x3, só no visivel) 

densidade desejada
modoDiluicao
diluicaoTotal
pwms

SetCO2
ModoGas

Compressor
Ventoinha
Agitador

modoGeral

-----
cabeçalho (incluir no LVP):
parametros pid
config TSL


pendencias:
- corrente peltier
- calibracao atlas
- calibracao densidade
- funcao alterar parametros
- diluicao (remover funcoes do modo componentes?)
- incluir gas
- incluir m1 e m2 no lvp no modo componentes
----------------------------------------
 *  
 * 
 */

byte Versao = 0;
byte ID = 0;

byte ModoDeOperacao = 0; // 0 para leituras apenas quando solicitadas; 1 para leituras automaticas dentro do dt; 2 para o modo "componentes"

/*
 * Versão 6: 
 *      geral: inclusao da funcao "alterarParametros" (para dar inicio a um experimento pre estabelecido)
 *      diluicao: inclusão de um delay entre o desligamento do compressor e a remocao das algas
 *                inclusao da função de drenagem
 *      painel: inclusao da variavel IncrementoBrilho no LVP          
 * 
 * Versão 5: 
 *      atuadores: aumento da resolucao (0 - 100, int -> float)
 */


/* Vale a pena aumentar o parametro no calculo do tempo de diluicao?        
 *         
 * Inserir ponteiros e eliminar Valor[x]        
 *         
 * Peltier:
 *  - adicionar o modo step
 *  - leitura da corrente?
 *  
 * Diluição: 
 *  - Escolher parâmetro
 *  - Quais serão os possíveis modos de diluição?
 *  - diluicao manual (para alcancar uma densidade escolhida)
 * 
 * Painel:
 *  - modo senoidal com periodo (e fase) variavel
 *  
 * Geral: 
 *  - Queremos assistente?
 *  - inserir funcao para pausar o reator
 * 
 */

/*
 * Coisas a serem verificadas:
 * - A ventoinha esta ligando/desligando de acordo com o peltier?
 * - As variaveis estao sendo realmente alteradas via lvp ou apenas cada componente de vetor?
 * - a leitura de ph esta compensando a temperatura?
 * 
 */

 
