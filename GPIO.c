/*
 * GPIO.c
 *      Author: Jorge Saloma
 */
#include "lib/include.h"
extern void Configurar_GPIO(void)
{
    SYSCTL->RCGCGPIO |=(1<<5); 
    GPIOF->LOCK= 0x4C4F434B;   // 2) unlock GPIO Port F
    GPIOF->CR = 0x1F;           // allow changes to PF4-0
    GPIOF->AMSEL = 0x00;        // 3) disable analog on PF
    GPIOF->PCTL = 0x00000000;   // 4) PCTL GPIO on PF4-0
    GPIOF->DIR = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
    GPIOF->AFSEL = 0x00;        // 6) disable alt funct on PF7-0
    GPIOF->PUR = 0x11;          // enable pull-up on PF0 and PF4
    GPIOF->DEN = 0x1F;          // 7) enable digital I/O on PF4-0
}

extern void Delay(void)
{
  unsigned long volatile time;
  time = 1600000;
  while(time)
  {
		time--;
  }
}

/*extern void Configurar_GPIO(void)
{
    //Se habilita el puerto a usar 
    SYSCTL->RCGCGPIO |=(1<<0);  //pueto A
    //Desabilita la función analogica del puerto para funcionar como GPIO pag. 687
    GPIOA->AMSEL = 0x00; 
    //Indicar que los pines trabajan como GPIO on PA7-0
    GPIOA->PCTL = 0x00000000;   
    //Indicar entradas y salidas. 0 -> entrada, 1 -> salida 
    GPIOA->DIR = (0<<2) | (0<<3) | (0<<4) | (0<<5) | (0<<6) | (0<<7); // PA2 - PA7 son entradas 
    //Se desabilita la función alternativa  PA7-0
    GPIOA->AFSEL = 0x00;   
    // Para habilitar que el edo. inicial de los pines sea pull-up O pull-down pag. 678
    GPIOA->PUR = (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5);// PA5 - PA0
    //Se habilitan los pines como digitales PA5-PA0
    GPIOA->DEN = (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5);     
    //pag. 667 Se limpian los pines a los que se asocian las interrupciones para poderlas configurar 
    GPIOA -> IM |= (0<<0) | (0<<1) | (0<<2) | (0<<3) | (0<<4) | (0<<5);
    //pag. 664 Configurar que sea sensible al borde 
    GPIOA -> IS |= (0<<0) | (0<<1) | (0<<2) | (0<<3) | (0<<4) | (0<<5);
    //pag. 665 registro para indicar que el IEV va a configurar que flancos detectar
    GPIOA -> IBE |= (0<<0) | (0<<1) | (0<<2) | (0<<3) | (0<<4) | (0<<5);
    //pag. 666 Indicar con que flanco trabaja 
    GPIOA -> IEV |= (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5); //Flanco de subida
    //pag. 668 establecer estado inicial
    GPIOA -> RIS |= (0<<0) | (0<<1) | (0<<2) | (0<<3) | (0<<4) | (0<<5);
    //PAG. 667 La interrupción del pin es enviada al controlador de interrupcion.
    GPIOA -> IM |= (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5);
    //pag. 104 Configurar la interrupción correspondiente al puerto A -> int 0
    // int. 3       2      1    0
    //    [4n+3] [4n+2] [4n+1] [4n] ---> [4n] = 0(valor de la int) : n=0
    //      FF     FF     FF    00    
    NVIC -> IP[0] = (NVIC ->IP[0]&0xFFFFFF00) | 0x00000002; 
    // Habilitar interrupción pag. 142 
    NVIC -> ISER[0] = 0x00000001; 
}

extern void GPIOA_INT_ISR(void)
{
    if (GPIOA ->RIS == (1<<0))
    {
      //Adquisición del ADC 0 y sec. 0
      ADC0->PSSI |= (1<<0);
      while((ADC0->RIS&0x01)==0){}; 
      Result[2] = ADC0->SSFIFO0&0xFFF; 
      ADC0->ISC = 0x0002;  
    }
    if (GPIOA ->RIS == (1<<0))
    {
      //Adquisición del ADC 0 y sec. 0
      ADC0->PSSI |= (1<<0); 
      while((ADC0->RIS&0x01)==0){}; 
      Result[1] = ADC0->SSFIFO0&0xFFF; 
      ADC0->ISC = 0x0002;  
    }
    if (GPIOA ->RIS == (1<<2))
    {
      //Adquisición del ADC 0 y sec. 0
      ADC0->PSSI |= (1<<0); 
      while((ADC0->RIS&0x01)==0){}; 
      Result[0] = ADC0->SSFIFO0&0xFFF;
      ADC0->ISC = 0x0002;  
    }
    if (GPIOA ->RIS == (1<<3))
    {
      //Adquisición del ADC 1 y sec. 2 
      ADC1->PSSI |= (1<<2); 
      while((ADC1->RIS&0x04)==0){}; 
      Result_2[1] = ADC1->SSFIFO2&0xFFF; 
      ADC1->ISC = 0x0004;  
    }
    if (GPIOA ->RIS == (1<<4))
    {
      //Adquisición del ADC 1 y sec. 2 
      ADC1->PSSI |= (1<<2); 
      while((ADC1->RIS&0x04)==0){}; 
      Result_2[0] = ADC1->SSFIFO2&0xFFF; 
      ADC1->ISC = 0x0004;    
    }
    if (GPIOA ->RIS == (1<<5))
    {
      //Adquisición del ADC 1 y sec. 3 
      ADC1->PSSI |= (1<<3); 
      while((ADC1->RIS&0x08)==0){}; 
      Result_3[0] = ADC1->SSFIFO3&0xFFF; 
      ADC1->ISC = 0x0008;  
    }
    GPIOA ->ICR |=(1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5); 
}*/