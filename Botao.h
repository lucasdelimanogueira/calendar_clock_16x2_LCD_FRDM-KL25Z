#include <MKL25Z4.h>

class Botao {

	private:
		
	public:
		void botao_init(){
		SystemCoreClockUpdate();
		
		//ativar clock no PORTA A (9)
		SIM_SCGC5 |= (1<<9);
			
						
		//configurar port como GPIO (8) (MUX)
		PORTA_PCR1 = (1<<8);
		PORTA_PCR2 = (1<<8);
		PORTA_PCR4 = (1<<8);
			
		//habilitando interrupções nos botoes (IRQC)
		PORTA_PCR1 |= (0<<16)|(1<<17)|(0<<18)|(1<<19);
		PORTA_PCR2 |= (0<<16)|(1<<17)|(0<<18)|(1<<19);
		PORTA_PCR4 |= (0<<16)|(1<<17)|(0<<18)|(1<<19);		
		NVIC_EnableIRQ(PORTA_IRQn);	
			
		//	//habilitando resistor pull up (1) (PE)
		PORTA_PCR1 |= (1<<1);
		PORTA_PCR2 |= (1<<1);
		PORTA_PCR4 |= (1<<1);
			
		
		}
		

	
};

