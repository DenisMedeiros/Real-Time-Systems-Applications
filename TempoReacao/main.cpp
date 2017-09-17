/**
 * Autor: Tiago Fernandes de Miranda
 * Universidade Federal do Rio Grande do Norte
 */

 #include <iostream>
 #include <pthread.h>
 #include <math.h>
 #include <unistd.h>
 #include "BlackTime.h" 
 #include "BlackGPIO/BlackGPIO.h"
 #include "ADC/Adc.h"
 #include <stdlib.h> 

 // Inclua as classes que achar necessario

#define UNIT_MS 1000;
#define UNIT_SEC 1000000;
using namespace std;

int main(int argc, char * argv[])
{
    cout << "Programa na Beagle Board STR UFRN" << endl;
    
    /****************** Instanciação de Variaveis ***********************/ 
    //GI/O
        BlackLib::BlackGPIO entrada(BlackLib::GPIO_47,BlackLib::input, BlackLib::SecureMode);
        BlackLib::BlackGPIO saida(BlackLib::GPIO_27,BlackLib::output, BlackLib::SecureMode);
    
    //Time Variaveis
        int temporandomico;
        Black::BlackTime tempoinicial();
        Black::BlackTime tempofinal();
        Black::BlackTime tempototal();

    /****************** Fim da Instanciação de Variaveis ***********************/    
    // Configurações Iniciais
    
    while(1){
        saida.setValue(BlackLib::high);// Configuração Inicial
        if(entrada.getIntValue()=="1"){
            saida.setValue(BlackLib::low);// Desligando a Luz para o inicio do Jogo
                // Tempo Random
                temporandomico = rand() % 3 + 1;
                long tempo = temporandomico*UNIT_SEC;
                usleep(tempo);
                
                timeinicial.getCurrentTime(); // Pega tempo inicial
                saida.setValue(BlackLib::high); // Acende a Luz
                
                // Espera o aperto do Botão
                while(!entrada.getIntValue()=="1"){
                    saida.setValue(BlackLib::low);
                }

                timefinal.getCurrentTime();// Pega o tempo Final
               
                tempototal=tempofinal-tempoinicial;// Pega o tempo total de reação
        }

    }

    return 0;
}

