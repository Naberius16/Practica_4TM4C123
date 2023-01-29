#include "lib/include.h"

int main(void)
{
    char valor1[4]; //Enviar los valores del ADC en carácteres
    char valor2[4];
    char valor3[4];
    char valor4[4];
    char valor5[4];
    char valor6[4];
    uint16_t Result[6];
    uint16_t lectura_0;
    Configurar_PLL(_20MHZ);  //**Configura velocidad de reloj
    Configura_Reg_ADC();
    Configurar_UART0();
    Configurar_GPIO();

    /*Usando el modulo 0 y 1 , configurar la tarjeta a la frecuencia asignada, para adquirir 6 señales analógicas 
    a una velocidad de 1 millón de muestras por segundo, por los canales asignados  y guardar los valores en un 
    arreglo para ser enviados con un botones  externos asociado al gpio A a través del protocolo de comunicación 
    asíncrona a una velocidad todo esto usando interrupciones.*/

    while(1)
    {   
        ADC0_InSeq1(Result);
    
        //Se convierte result(numerico) en valor(caracteres) con una base de 10 (en binario)
        //itoa(Result[0],valor1,10); 
        lectura_0 = (uint16_t)(Result[0]);
        integer_to_char(lectura_0);

        //Se manda el valor de la variable por uart
        printString(valor1);
    }
}