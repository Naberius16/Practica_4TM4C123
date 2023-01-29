
#include "lib/include.h"

extern void Configurar_UART0(void)
{
    //Paso 1 (RCGCUART) pag.344 UART/modulo0 0->Disable 1->Enable
    SYSCTL->RCGCUART  = (1<<0);   
    //Paso 2 (RCGCGPIO) pag.340 Enable clock port A
    SYSCTL->RCGCGPIO |= (1<<0);     
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOA->AFSEL = (1<<1) | (1<<0); //PA0 y PA1
    //GPIO Port Control (GPIOPCTL) PA0-> U0Rx PA1-> U0Tx pag.1351 para conocer que valor poner
    GPIOA->PCTL = (GPIOA->PCTL&0xFFFFFF00) | 0x00000011;// (1<<0) | (1<<4);//0x00000011
    // Habilitan los pines como digital pag.682
    GPIOA->DEN = (1<<0) | (1<<1);//PA1 PA0
    //Indicar entradas y salidas pag. 895 
            //      Rx(in) | Tx(out)
    //GPIOC -> DIR |= (0<<0) | (1<<1); //salida -> 1, entrada -> 0
    //Desabilita-Limpia los registros pag. 918
    UART0->CTL = (0<<9) | (0<<8) | (0<<0); //bit 9 -> Tx, bit 8 -> Rx, bit 0 -> uart.
    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = 33,000,000 / (16*19200) = 107.42
    UARTFBRD[DIVFRAC] = integer(.42 * 64 + 0.5) = 27.38 = 28
    */
    UART0->IBRD = 107;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART0->FBRD = 28;
    //  UART Line Control (UARTLCRH) pag.916
    UART0->LCRH = (0x3<<5)|(1<<4); //Para que se tengan 8 bits, y |(1<<4); activar el bit de paridad pero solo con el uart0
    //  UART Clock Configuration(UARTCC) pag.939
    UART0->CC =(0<<0);
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART0->CTL = (1<<0) | (1<<8) | (1<<9);
}
//Para enviar dato/caracter
extern void printChar(char c)
{
    while((UART0->FR & (1<<5)) != 0 );
    UART0->DR = c;
}

//Enviar datos en cadena
extern void printString(char* string)
{
    while(*string)
    {
        printChar(*(string++));
    }
    printChar('\n');
}
