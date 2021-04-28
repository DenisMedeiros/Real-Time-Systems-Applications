#include <iostream>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h> 
#include <time.h>  
#include "BlackGPIO/BlackGPIO.h"
#include "ADC/Adc.h"
#include <chrono>
#include <iomanip>
#include <stdint.h>
#include "display.h"

#define UNIT_MS 1000
#define UNIT_SEC 1000000

using namespace std;
																																																																	

int main(int argc, char * argv[])
{   

    BlackLib::BlackGPIO led(BlackLib::GPIO_67, BlackLib::output, BlackLib::SecureMode);
    BlackLib::BlackGPIO botao(BlackLib::GPIO_68, BlackLib::input, BlackLib::SecureMode);
    uint32_t tempoAcender, tempoReacao; /* Ambos em milissegundos */
    clock_t inicio, fim;
    
	Display display(BlackLib::GPIO_65, BlackLib::GPIO_45, 
		BlackLib::GPIO_69, BlackLib::GPIO_60, BlackLib::GPIO_27, BlackLib::GPIO_66, 
		BlackLib::GPIO_49, BlackLib::GPIO_115, 
		BlackLib::GPIO_20, BlackLib::GPIO_47, 
		BlackLib::GPIO_48, BlackLib::GPIO_46);
		
	
    while(1)
    {
		/* Fornece o seed para o PRNG. */
		srand (time(NULL));
		
		/* Aplicação inicia com o LED acesso. */
		led.setValue(BlackLib::high);
		
		/* Limpa o display. */
		display.showNumber(0000);
		
		/* Aguarda o usuário pressionar o botão pela primeira vez. */
		while(stoi(botao.getValue()) == 1);	
		
		/* O led apaga. */
		led.setValue(BlackLib::low);
		
		/* Gera um número aleatório entre 0 e 3000 milissegundos. */
		tempoAcender = rand() % 3000;
		
		/* Espera o tempo acima. */
		usleep(tempoAcender * UNIT_MS);
		
		/* O LED reacende */
		led.setValue(BlackLib::high);
		
		/* Mede o tempo de reação do usuário. */
		inicio = std::clock();
		while(stoi(botao.getValue()) == 1);
		fim = std::clock();
		
		/* Exibe o tempo de reação total. */ 
	
		tempoReacao = (fim-inicio) / (CLOCKS_PER_SEC / 1000);
		cout << "Tempo de acender foi de : " << tempoAcender << " ms " << endl;
		cout << "Tempo de reação: " << tempoReacao << " ms." << endl;
		display.showNumber(tempoReacao);
		led.setValue(BlackLib::low);
		cout << "Reiniciando a contagem em 5 segundos..." << endl;
		usleep(5 * UNIT_SEC);
		
    }
    return 0;
}
