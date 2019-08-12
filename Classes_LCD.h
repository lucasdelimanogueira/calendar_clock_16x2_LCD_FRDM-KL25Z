//#include <iostream>
#include <MKL25Z4.h>



char data[16];//vetor onde sera armazenado data pra escrever no LCD
char horario[16];//vetor onde sera armazenado horario pra escrever no LCD


//constantes
#define up 1 //estado logico 1
#define down 0 //estado logico 0
#define RS 0 //pino da placa que sera conectado o RS do LCD
#define	E 3 //pino da placa que sera conectado o Enable do LCD
#define	DB0 4 //pino da placa que sera conectado o DB0 do LCD
#define	DB1 5
#define DB2 6
#define DB3 7
#define DB4 8
#define DB5 9
#define DB6 10 
#define DB7 11
		
class LCD //Definir classe LCD
{
	private:		
			
		
	public:
		
////////atualizar LCD subindo e descendo o enable (falta colocar utilizando interrupção ao invés de delay)
			void LCDatualizar(){
			
			//clock (enable) sobe e desce p atualizar
			for(int k=0;k<10000;k++){ //delay
			}	
			PTC->PSOR = (1<<E); //pino 3 setado como 1
			for(int k=0;k<10000;k++){ //delay
			}	
			PTC->PCOR = (1<<E); //pino 3 resetado como 0
			for(int k=0;k<10000;k++){ //delay
			}	
			}
//////////////////////////////////////////////////////////


			
			
////////inicializar LCD////////
		void LCDinit(void){
			//configurando clock
			SystemCoreClockUpdate();
			SysTick_Config(SystemCoreClock/1000-1);
			//clock na porta C
			SIM_SCGC5 |= (1<<11);
			
			//configurando PORTC como gpio
			PORTC_PCR0 = (1<<8);
			PORTC_PCR3 = (1<<8);
			PORTC_PCR4 = (1<<8);
			PORTC_PCR5 = (1<<8);
			PORTC_PCR6 = (1<<8);
			PORTC_PCR7 = (1<<8);
			PORTC_PCR8 = (1<<8);
			PORTC_PCR9 = (1<<8);
			PORTC_PCR10 = (1<<8);
			PORTC_PCR11 = (1<<8);
			
			//configurando pinos como saida (saida = 1 / entrada = 0)
			PTC->PDDR = (0x0FF9);			
			
			// function set;
			PTC->PDOR = (0x03C<<4);
			LCDatualizar();
			
			//display on
			PTC->PDOR = (0x0F<<4);
			LCDatualizar();
			
			
			//clear display
			PTC->PDOR = (0x01<<4);
			LCDatualizar();
			
			//go home
			PTC->PDOR = (0x02<<4);
			LCDatualizar();
		}
//////////////////////////////////////////////////

		
		
////////escrever um string no LCD//////////
		void LCDwrite(char c[]){
				int index = 0;//contador pra posição do vetor
				while (c[index] != '\0'){//enquanto ainda houver caracteres na string, mandar esse caractere para o LCD através do registrador PDOR
				PTC->PDOR =(c[index]<<4)|1;
				LCDatualizar();
				index++;
				}
		}
/////////////////////////////////////////////////		
	
		
		
		
/////////////////mandar cursor pra determinada posição///////////////////////////////	
		void LCDgoto(int linha, int coluna){
		
		//configurando pra mandara os bits pro PORTC que executam a função Cursor or Display Shift
		PTC->PDOR = 0;//zerando todos valores do PORTC
		PTC->PDOR = (1<<5);//desloca 1 pra posição 5 do PORTC
		LCDatualizar();
		PTC->PDOR = (1<<6)|(1<<8);
			
			
			//com o PORTC com os bits que executam a função Cursor or Display Shift definidos
			//basta atualizar o display o numero de vezes que se quer que o cursor ande
			//a cada LCDatualizar() o cursor anda uma posição
			if (linha == 1){	
				for(int i = 0;i<coluna-1;i++){
					LCDatualizar();
				}
			}
			else if (linha == 2){
				for(int i=0;i<coluna+39;i++){
					LCDatualizar();
				}	
			}
		}
/////////////////////////////////////////////////////////////////
		
		
		
		
///////////////escrever data e hora////////////////////////
	void LCDescreverdata(int dia, int mes, int ano, int hora, int min, int seg){
		
		//cursor go home
		PTC->PDOR = (0x02<<4);
		LCDatualizar();
		
		//Como a função LCDescreverdata recebe valores inteiros como input
		//e a função LCDwrite, que escreve valores, tem como entrada char
		//devemos transformar esses valores inteiros nos correspondentes char	
		
		//transformando int XX em char XX
		data[0] = dia/10 + 48;
		data[1] = dia%10 + 48;
		
		data[2] = '/';
		
		data[3] = mes/10 + 48;
		data[4] = mes%10 + 48;
		
		data[5] = '/';
		
		//rotina pra transformar int XXXX em um char XXXX
		int resto = 0;
		for(int i=9;i>=6;i--){
		resto = ano%10 + 48;
		data[i] = resto;
		ano = ano/10;
		}
		
		//escrever o vetor data (que contem dia mes e ano) no LCD
		LCDwrite(data);
		
		LCDgoto(2,2);//ir pra posição embaixo, para começar a escrever a hora na segunda linha
		
		//transformando int XX em char XX
		horario[0] = hora/10 + 48;
		horario[1] = hora%10 + 48;
		
		horario[2] = ':';
		
		horario[3] = min/10 + 48;
		horario[4] = min%10 + 48;
		
		horario[5] = ':';
		
		horario[6] = seg/10 + 48;
		horario[7] = seg%10 + 48;
		
		LCDwrite(horario);//escrever vetor horario no LCD
	}
	
///////////////////////////////////////////////////////
	
	
	
	
/////////////piscar a posição dos Segundos (pra quando tiver no menu de setup dos segundos)
	void piscaSeg(){
		PTC->PDOR = (0x02<<4);//cursor go home
		LCDatualizar();
		LCDgoto(2,2);//cursor ir pra posição do horario
		horario[6] = ' ';//escrever espaço no primeiro digito dos segundos
		horario[7] = ' ';//escrever espaço no segundo digito dos segundos
		LCDwrite(horario);
	}
////////////////////////////////////////////////////



	
/////////////////piscar a posição dos Minutos (pra quando tiver no menu de setup dos minutos)
	void piscaMin(){
		PTC->PDOR = (0x02<<4);//cursor go home
		LCDatualizar();
		LCDgoto(2,2);//cursor ir pra posição do horario
		horario[3] = ' ';//escrever espaço no primeiro digito dos minutos
		horario[4] = ' ';//escrever espaço no segundo digito dos minutos
		LCDwrite(horario);
	}
////////////////////////////////////////////////////
	

	

//////////////////////piscar a posição da Hora (pra quando tiver no menu de setup da hora)	
	void piscaHora(){
		PTC->PDOR = (0x02<<4);//cursor go home
		LCDatualizar();
		LCDgoto(2,2);//cursor ir pra posição do horario
		horario[0] = ' ';//escrever espaço no primeiro digito da hora
		horario[1] = ' ';//escrever espaço no segundo digito da hora
		LCDwrite(horario);
	}
		
};



