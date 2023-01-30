/*
 * ADC.c
 *
 *  Created on: 28/12/2022
 *      Author: Jorge Saloma
 */
#include "lib/include.h"
extern void Configura_Reg_ADC(void)
{
/*
    Usando el modulo 0 y 1 , configurar la tarjeta a la frecuencia asignada, para adquirir 6 señales analógicas 
    a una velocidad de 1 millón de muestras por segundo, por los canales asignados  y guardar los valores en un 
    arreglo para ser enviados con un botones  externos asociado al gpio D a través del protocolo de comunicación 
    asíncrona a una velocidad todo esto usando interrupciones.

                                                 PIN - CANAL
    d) 11,9,6,7,4,2 33MHZ 19200 -sec3,sec0,sec2: PB5 - AIN11, PE4 - AIN9, PD1 - AIN6, PD0 - AIN7, PD3 - AIN4, PE1 - AIN2
                           pag. 105                  11           9           6           7           4           2
    modulo 0 -> sec. 0 -> Interrupción 48
    modulo 1 -> sec. 3 -> Interrupción 51
    modulo 1 -> sec. 2 -> Interrupción 50
    */
    //Pag 352. Inicializar el reloj del ADC (RCGCADC)
    SYSCTL->RCGCADC = (1<<0)|(1<<1);  //Habilitar el modulo 1 y 0
    //Pag 340.Habilitar puertos (RGCGPIO). Se habilitan canales del adc pág. 801 para canales y puerto
    //                     F     E      D       C      B     A     
    SYSCTL->RCGCGPIO |= (1<<5)|(1<<4)|(1<<3)|(0<<2)|(0<<1)|(1<<0); //Port A -> UART, Port F -> leds, Port E y D para canales 
    //Pag 663 (GPIODIR) Habilta los pines como I/O un 0 <- entrada y 1 <- salida. Pag 801
    GPIOB_AHB->DIR = (0<<5); // PB5
    GPIOE_AHB->DIR = (0<<1) | (0<<5); // PE1 y PE5
    GPIOD_AHB->DIR = (0<<0) | (0<<1) | (0<<3); // PD0, PD1 y PD3
    //(GPIOAFSEL) pag.672 Indicar que esos pines trabajan con función alternativa 
    GPIOB_AHB->AFSEL = (1<<5); // PB5
    GPIOE_AHB->AFSEL = (1<<1) | (1<<5); // PE1 y PE5
    GPIOD_AHB->AFSEL = (1<<0) | (1<<1) | (1<<3); // PD0, PD1 y PD3
    //(GPIODEN) pag.683 desabilita el modo digital de los pines porque son analogicos
    GPIOB_AHB->DEN = (0<<5); // PB5
    GPIOE_AHB->DEN = (0<<1) | (0<<5); // PE1 y PE5
    GPIOD_AHB->DEN = (0<<0) | (0<<1) | (0<<3); // PD0, PD1 y PD3
    //Pag 688 GPIOPCTL registro combinado con el GPIOAFSEL y la tabla pag 1351, Se indica la función alternativa de los pines
    GPIOB_AHB->PCTL = GPIOB_AHB->PCTL & (0XFF0FFFFF); //Poner 0 en los bits del pin 5
    GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0xFF0FFF0F); //Poner 0 en los bits de los pines 1 y 5
    GPIOD_AHB->PCTL = GPIOD_AHB->PCTL & (0xFFFF0F00); //Poner 0 en los bits de los pines 0, 1 y 3 
    //(GPIOAMSEL) pag.687 habilitar analogico los pines de los canales
    GPIOB_AHB->AMSEL = (1<<5); // PB5
    GPIOE_AHB->AMSEL = (1<<1) | (1<<5); // PE1 y PE5
    GPIOD_AHB->AMSEL = (1<<0) | (1<<1) | (1<<3); // PD0, PD1 y PD3
    //Pag 891 El registro (ADCPC) establece la velocidad de conversión por segundo
    ADC0->PC = (0<<3)|(1<<2)|(1<<1)|(1<<0);//1Mms  Se pone 7 en binario para trabajar con la max. velocidad
    ADC1->PC = (0<<3)|(1<<2)|(1<<1)|(1<<0);//1Mms
    //Pag 841 Este registro (ADCSSPRI) configura la prioridad de los secuenciadores 
    ADC0->SSPRI = 0x0231; // Prioridad 0 -> sec 3,  1 -> sec.0, 2-> sec. 2, 3 -> sec. 1
    ADC1 ->SSPRI = 0x1032;
    //Pag 821 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0); //Se desactivan los 4 secuenciadores para configurar
    ADC1->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0);
    //Pag 834 Este registro (ADCEMUX) selecciona el evento que activa la conversión (trigger)
    ADC0->EMUX  = (0x0000); // Por procesador. *confg. antes:  Sec. 0 se da un valor de 4 para configurar con interrupciones (External GPIO)
    ADC1->EMUX  = (0x0000); // Sec. 2 y 3
    //Pag 867 Este registro (ADCSSMUX2) define las entradas analógicas con el canal y secuenciador seleccionado
    ADC0->SSMUX0 = 0x00000679; // Los canales 6,7,9 se definen para el mux 0 del sec. 0 en el modulo 0
    ADC1->SSMUX2 = 0x0024; // Los canales 2,4  se definen para el mux 2 del sec. 2 en el modulo 1
    ADC1->SSMUX3 = 0xB;  // El canal 11 se define para el mux 3 del sec. 3 en el modulo 1 
    //pag 868 Este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción
    //    Sec. 0        mux 0            mux 1             mux 2      
    ADC0->SSCTL0 = (1<<2) | (1<<1)| (1<<6) | (1<<5) | (1<<10) | (1<<9); //Configuración y finalización de muestreo
    //    Sec. 2        mux 0            mux 1 
    ADC1->SSCTL2 = (1<<2) | (1<<1)| (1<<6) | (1<<5);
    //    Sec. 3        mux 0  pag. 876
    ADC1->SSCTL3 = (1<<2) | (1<<1);
    // Pag 825 - Interrumpe la conversión del secuenciador a usar 
    ADC0->IM |= (1<<0); /* Unmask ADC0 sequence 2 interrupt*/ 
    ADC1->IM |= (1<<2) | (1<<3); // Sec. 2 -> 1. Sec. 3 -> 1 
    //Pag 821 (ADCACTSS) Activación de los secuenciadores utilizados 
    ADC0->ACTSS = (0<<3) | (0<<2) | (0<<1) | (1<<0); // se habilita el sec. 0
    ADC1->ACTSS = (1<<3) | (1<<2) | (0<<1) | (0<<0); // se habilita el sec. 2 y 3 
    ADC0->PSSI |= (1<<0); //Inicializa el sec. 0 cuando se configura el ADC en modo de procesador
    ADC1->PSSI |= (1<<2) | (1<<3); //Inicializa el sec. 2 y 3
}

extern void ADC0_InSeq1(uint16_t Result[6])
{
       ADC0->PSSI = 0x00000002;
       while((ADC0->RIS&0x02)==0){}; 
       Result[0] = ADC0->SSFIFO1&0xFFF; 
       Result[1] = ADC0->SSFIFO1&0xFFF;
       Result[2] = ADC0->SSFIFO1&0xFFF;
       ADC0->ISC = 0x0002; 
}

extern void ADC1_InSeq2(uint16_t Result[6])
{
       ADC1->PSSI = 0x00000004;
       while((ADC1->RIS&0x04)==0){}; 
       Result[3] = ADC1->SSFIFO2&0xFFF; 
       Result[4] = ADC1->SSFIFO2&0xFFF; 
       ADC1->ISC = 0x0004;  
}

extern void ADC1_InSeq3(uint16_t Result[6])
{
       ADC1->PSSI = 0x00000008;  
       while((ADC1->RIS&0x08)==0){}; 
       Result[5] = ADC1->SSFIFO3&0xFFF; 
       ADC1->ISC = 0x0008;
}