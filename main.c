#include "lib/include.h"
extern void integer_to_char(uint16_t number) //lo mismo que itoa
{
    char number_in_string[5];
    int i = 3;
    int j = 0; 
    int n_digit;  
    while (i >= 0){
        n_digit = (int)number / (int)(pow(10, i));
        number_in_string[j] = n_digit + '0';
        number = number - (n_digit * pow(10, i)); 
        i = i - 1; 
        j++;
    }
    number_in_string[4] = '\n';
    printString(number_in_string);
}

int main(void)
{
    uint16_t Result[6]; //variables
    uint16_t valor1; //es tipo char porque se envian por el UART
    uint16_t valor2;
    uint16_t valor3;
    uint16_t valor4;
    uint16_t valor5;
    uint16_t valor6;
    
    Configurar_PLL(_20MHZ); //configuracion de la velocidad de reloj 
    Configura_Reg_ADC(); //configuracion del modulo ADC
    Configurar_UART0(); //configuración del UART 

    //Configurar_GPIO();  //falta configuración GPIO
    
    while(1) 
    {
        ADC0_InSeq1(Result); //llamada a la conversion por procesador}
        ADC1_InSeq2(Result);
        ADC1_InSeq3(Result);
        
        valor1 = (uint16_t)(Result[0]);
        valor2 = (uint16_t)(Result[1]);
        valor3 = (uint16_t)(Result[2]);
        valor4 = (uint16_t)(Result[3]);
        valor5 = (uint16_t)(Result[4]);
        valor6 = (uint16_t)(Result[5]);

        integer_to_char(valor1); //funcion para convertir de valor numérico a char
        integer_to_char(valor2); 
        integer_to_char(valor3); 
        integer_to_char(valor4);
        integer_to_char(valor5);
        integer_to_char(valor6);

        /*valor1=(float)(((Result[0]))*3.3)/4096;
        valor2=(float)(((Result[1]))*3.3)/4096;
        valor3=(float)(((Result[2]))*3.3)/4096;

        ADC1_InSeq2(Result); 
        valor4=(float)(((Result2[0]))*3.3)/4096;
        valor5=(float)(((Result2[1]))*3.3)/4096;
        valor6=(float)(((Result2[2]))*3.3)/4096;*/
    }
}
/*int main(void)
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
    Configurar_GPIO();*/

    /*Usando el modulo 0 y 1 , configurar la tarjeta a la frecuencia asignada, para adquirir 6 señales analógicas 
    a una velocidad de 1 millón de muestras por segundo, por los canales asignados  y guardar los valores en un 
    arreglo para ser enviados con un botones  externos asociado al gpio A a través del protocolo de comunicación 
    asíncrona a una velocidad todo esto usando interrupciones.*/

    /*while(1)
    {   
        ADC0_InSeq1(Result);
    
        //Se convierte result(numerico) en valor(caracteres) con una base de 10 (en binario)
        //itoa(Result[0],valor1,10); 
        lectura_0 = (uint16_t)(Result[0]);
        integer_to_char(lectura_0);

        //Se manda el valor de la variable por uart
        printString(valor1);
    }
}*/