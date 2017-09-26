#include <iostream>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include "BlackTime/BlackTime.h" 
#include "BlackGPIO/BlackGPIO.h"
#include "ADC/Adc.h"
#include <stdlib.h> 
#include <stdint.h>
#include <time.h>
#include "display.h"

 // Inclua as classes que achar necessario

#define UNIT_MS 1000
#define UNIT_SEC 1000000

using namespace std;

int main(int argc, char * argv[])
{
	/****************** Instanciação de Variaveis ***********************/ 
	// GPIO
    BlackLib::BlackGPIO saida(BlackLib::GPIO_67, BlackLib::output, BlackLib::SecureMode);
    BlackLib::BlackGPIO entrada(BlackLib::GPIO_68, BlackLib::input, BlackLib::SecureMode);

	// Variáveis relacioandas ao tempo
	uint32_t tempoRandomico, milissegundos;
	BlackLib::BlackTime medidor;
	BlackLib::BlackTimeElapsed tempoDecorrido;
	
	// Display
	Display display(BlackLib::GPIO_65, BlackLib::GPIO_45, 
		BlackLib::GPIO_69, BlackLib::GPIO_60, BlackLib::GPIO_27, BlackLib::GPIO_66, 
		BlackLib::GPIO_49, BlackLib::GPIO_115, 
		BlackLib::GPIO_20, BlackLib::GPIO_47, 
		BlackLib::GPIO_48, BlackLib::GPIO_46);

	/****************** Fim da Instanciação de Variaveis ***********************/    

	while(1){
			
		// Fornecendo o seed
		srand (time(NULL));
		
		// Configuração inicial
		saida.setValue(BlackLib::high);
		display.showNumber(0000);
		
		while(entrada.getValue() == "1");
		
		// Desligando a Luz para o inicio do Jogo
		saida.setValue(BlackLib::low);
		
		// Gera um número aleatório entre 1 e 3 segundos e espera este tempo.
		tempoRandomico = rand() % 3000 + 1000;
		usleep(tempoRandomico * UNIT_MS);

		// Pega o tempo inicial e acende a luz
		medidor.start(); 
		saida.setValue(BlackLib::high);

		// Espera o aperto do Botão
		while(entrada.getValue() == "1");

		// Pega o tempo final
		tempoDecorrido = medidor.elapsed();
		milissegundos = tempoDecorrido.second*1000 + tempoDecorrido.miliSecond;
		
		
		display.showNumber(milissegundos);
		//cout << "Tempo de reação: " << milissegundos << endl;
		//cout << "Reiniciando a contagem em 5 segundos..." << endl;
		usleep(5 * UNIT_SEC);
	}

	return 0;
}

