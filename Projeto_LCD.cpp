#include "C:\Users\mecatronica\Documents\Lucas de Lima Sistemas Digitais 2\Projeto 1_LCD Versão C++\Projeto 1_LCD_Versão C++_Lucas de Lima Nogueira\Classes_LCD.h"
#include "C:\Users\mecatronica\Documents\Lucas de Lima Sistemas Digitais 2\Projeto 1_LCD Versão C++\Projeto 1_LCD_Versão C++_Lucas de Lima Nogueira\Botao.h"
#include <MKL25Z4.h>
#include <string.h>
//using namespace std;

int countMS = 0;//contador dos milissegundos
int now, now2, now3, now4, lastCount, lastCount2;//variaveis auxiliares pra contar variações de tempo
int flagUP, flagDOWN;


extern "C" {
void SysTick_Handler(void){//interrupção que é chamada a cada milissegundo
	countMS++;
}



void PORTA_IRQHandler(void){//interrupção para os botões
	if((PORTA_PCR1&(1<<24)) == (1<<24)){		
	PORTA_PCR1 |= (1<<24);
	}
	
	//se botao up
	if((PORTA_PCR2&(1<<24)) == (1<<24)){		
	flagUP = 1;
	PORTA_PCR2 |= (1<<24);
	}

	
	//se botao down
	if((PORTA_PCR4&(1<<24)) == (1<<24)){
	flagDOWN = 1;
	PORTA_PCR4 |= (1<<24);
	}
}

}
int min = 0;//variavel minuto
int seg = 0;//variavel segundo
int hora = 0;//variavel hora

int main(){
//	char a[32] = "olakaa, mundo";
	LCD lcd1;//instanciando o objeto lcd
	lcd1.LCDinit();//inicializando lcd
	Botao botoes;//instanciando o objeto botao
	botoes.botao_init();//inicializando botao
//	lcd1.LCDwrite(a);
//	lcd1.LCDgoto(1,8);
	lastCount = countMS;//armazena o tempo atual em lastCount


//menu 0 -> normal
//menu 1 -> setup dos segundos
//menu 2 -> setup dos minutos
//menu 3 -> setup da hora
int menu = 0;	
	
while(1){
	
	//rotina para varia o horario
	if (menu == 0){//se está na tela normal	
		now = countMS;
		if(now-lastCount >= 1000){//aumenta variavel segundo a cada 1000 milissegundos
		seg++;	
			
		if(min>=60){//quando minuto chegar em 60, zera minuto e aumenta uma hora
		min = 0;
		hora++;
		}		
		
		if(seg>=60){//quando segundo chegar em 60, zera segundo e aumenta minuto
		seg = 0;
		min++;
		}		
					
		lcd1.LCDescreverdata(3,5,1996,hora,min,seg);//escrever o horario no LCD	
		lastCount = now;//atualiza lastCount depois dos 1000 milissegundos
		}
		
		
		if ((PTA->PDIR&(1<<1)) == (1<<1)) {//se botao setup não tiver apertado
			now2 = countMS;
		}
		else {// botao setup apertado por 1s, entra em menu	dos segundos	
		 if(countMS-now2 >= 1000){
			menu = 1;
			now2 = countMS;
			now3 = countMS;
		 }		 
		}
		
	}	
		
		
		if (menu > 0) {//if dos menus
			
			// se dentro de menus eu apertar botao setup, mudar o menu pro lado (pra minuto, depois hora, etc)
			if ((PTA->PDIR&(1<<1)) == (1<<1)) {//se botao setup não tiver apertado
				now2 = countMS;				
					if(countMS-now3 >= 5000){//se ficar 5000 milissegundos sem apertar o botao de setup, sai do menu
						menu = 0;
					}
			}
		
			else {// botao setup apertado por 1s muda menu				
				if(countMS-now2 >= 1000){
					now3 = countMS;//reseta now3 pra nao sair do menu por 5s
					menu++;//vai pro menu do lado (minuto, hora, etc)
					if (menu > 3){
						now3 = countMS;//reseta now3 pra nao sair do menu por 5s
						menu = 0;//sai do menu
					}
					now2 = countMS;
				}		 
			}

			if (menu == 1){ //menu do segundo
				 if (seg >= 60){//se for aumentando segundo mais que 60, zera segundo 
							seg = 0;
							lcd1.LCDescreverdata(3,5,1996,hora,min,seg);
						}
				 if (seg < 0){//se for diminuindo segundo menos que zero, zera segundo
							seg = 0;
							lcd1.LCDescreverdata(3,5,1996,hora,min,seg);
						}	
						
				///////////// piscar //////////
											
				//se nenhum botao apertado	
			if (flagUP == 0 && flagDOWN == 0){
				//if ((PTA->PDIR&(1<<2)) == (1<<2) && (PTA->PDIR&(1<<4)) == (1<<4)){	
					now = countMS;
					now4 = countMS;
					
					
					if(now-lastCount >= 1000){
						now4 = countMS;
							if(now4-lastCount2 >=1000){
								lcd1.LCDescreverdata(3,5,1996,hora, min, seg);
								lastCount2 = now4;
								lastCount = now;
							}
								
						lcd1.piscaSeg();//piscar menu dos segundos
						lastCount = now;
					}	
					
			////////////////////////////
																						
				}
		//}
							
				//algum botao apertado 	
				else {
					
					if(flagUP == 1){
					// se botao up n tiver apertado
					//if ((PTA->PDIR&(1<<2)) == (1<<2)) {}
						
					//botao up apertado
					//else{
					now3 = countMS;//reseta now3 pra nao sair do menu por 5s
					seg++;//acrescenta segundo
					lcd1.LCDescreverdata(3,5,1996,hora,min,seg);			
					//}
					flagUP = 0;}
					
					
				if(flagDOWN == 1){	
				//botao down não tiver apertado				
				//if ((PTA->PDIR&(1<<4)) == (1<<4)){} 
					
				//botao down apertado		
				//else {		
					now3 = countMS;//reseta now3 pra nao sair do menu por 5s
					seg--;//decresce segundo					
					lcd1.LCDescreverdata(3,5,1996,hora,min,seg);					
				//}
				
				flagDOWN = 0;}
				
				
			}//fim algum botao apertado
	
		}//fim menu 1			
			
			
			//menu do minuto	
			if (menu == 2){  
				if (min >= 60){//se aumentar minuto mais que 60, zera minuto
								min = 0;
								lcd1.LCDescreverdata(3,5,1996,hora,min,seg);
							}
				if (min < 0){//se diminuir minuto menos que zero, zera minuto
								min = 0;
								lcd1.LCDescreverdata(3,5,1996,hora,min,seg);
							}					
				
				///////////// piscar //////////
							
				//se nenhum botao apertado				
				if ((PTA->PDIR&(1<<2)) == (1<<2) && (PTA->PDIR&(1<<4)) == (1<<4)){	
					now = countMS;
					now4 = countMS;
					
					
					if(now-lastCount >= 50){
						now4 = countMS;
							if(now4-lastCount2 >= 1000){
								lcd1.LCDescreverdata(3,5,1996,hora, min, seg);
								lastCount2 = now4;
								lastCount = now;
							}
								
						lcd1.piscaMin();//piscar digitos do minuto
						lastCount = now;
					}	
					
			////////////////////////////
				
					}
				
				//algum botao apertado 
				else {
						// se botao up n tiver apertado
						if ((PTA->PDIR&(1<<2)) == (1<<2)) {}

						//botao up apertado	
						else{
						now3 = countMS;//reseta now3 pra nao sair do menu por 5s
						min++;//acrescenta minuto
						lcd1.LCDescreverdata(3,5,1996,hora,min,seg);			
					}
						
					//botao down não tiver apertado
					if ((PTA->PDIR&(1<<4)) == (1<<4)){}
					
					//botao down apertado		
					else {	
						now3 = countMS;//reseta now3 pra nao sair do menu por 5s
						min--;//decresce minuto
						lcd1.LCDescreverdata(3,5,1996,hora,min,seg);					
					}
			}
		}
			
			
			//menu da hora		
			if (menu == 3){ 
					if (hora >= 24){//se aumentar hora mais que 24, zera hora
								hora = 0;
								lcd1.LCDescreverdata(3,5,1996,hora,min,seg);
					}
					if (hora < 0){//se diminuir hora menos que 0, zera hora
								hora = 0;
								lcd1.LCDescreverdata(3,5,1996,hora,min,seg);
					}						
					
					
					
					///////////// piscar //////////
					
					//se nenhum botao apertado	
					if ((PTA->PDIR&(1<<2)) == (1<<2) && (PTA->PDIR&(1<<4)) == (1<<4)){	
						now = countMS;
						now4 = countMS;
					
					
						if(now-lastCount >= 50){
							now4 = countMS;
								if(now4-lastCount2 >= 1000){
									lcd1.LCDescreverdata(3,5,1996,hora, min, seg);
									lastCount2 = now4;
									lastCount = now;
								}
									
							lcd1.piscaHora();
							lastCount = now;
						}	
							////////////////////////////
					
						}
				
					//algum botao apertado 
					else {
						// se botao up n tiver apertado		
						if ((PTA->PDIR&(1<<2)) == (1<<2)) {}
							
						//botao up apertado	
						else {
							now3 = countMS;//reseta now3 pra nao sair do menu por 5s
							hora++;//acrescenta hora
							lcd1.LCDescreverdata(3,5,1996,hora,min,seg);			
						}
						
						//botao down não tiver apertado		
						if ((PTA->PDIR&(1<<4)) == (1<<4)){}  
							
						//botao down apertado		
						else {				
							now3 = countMS;//reseta now3 pra nao sair do menu por 5s
							hora--;//decresce hora
							lcd1.LCDescreverdata(3,5,1996,hora,min,seg);					
						}
			}
		}
			
			//menu do mês
			if (menu == 4){
			}
			
			
		}//fecha do menu
		
		
	
	
}//fecha while
}//fecha int





	
//		int i;
//	SystemCoreClockUpdate();
//	SIM_SCGC5 = (1<<10); 
//	SIM_SCGC5 |= (1<<12); 
//	PORTB_PCR18 = (1<<8);
//	PORTB_PCR19 = (1<<8);
//	PORTD_PCR1 = (1<<8);

//	PTD->PDDR = (1<<1);
//	PTD->PDOR = (1<<1);
//	PTB->PDDR = (3<<18);

//	while(1){	
//		
//		PTD->PCOR = (1<<1);
//		for(i=0;i<10000000;i++){
//	}		
//	PTD->PSOR = (1<<1);
//	
//	PTB->PCOR = (1<<18); 
//	
//	for(i=0;i<10000000;i++){
//	}	
//	
//	PTB->PSOR = (1<<18); 
//	
//	PTB->PCOR = (1<<19); 	
//	
//	for(i=0;i<10000000;i++){
//	}	
//	PTB->PSOR = (1<<19); 	


//		}
//}


