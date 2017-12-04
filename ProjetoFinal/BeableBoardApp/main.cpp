#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h> // biblioteca pthread
#include <semaphore.h>

#include "adc.h"
#include "BlackTime/BlackTime.h"
#include "BlackGPIO/BlackGPIO.h"
#include "ADC/Adc.h"
#include "display.h"

#define UNIT_MS 1000
#define UNIT_SEC 1000000

#define BUFFER_SIZE 7

using namespace std;

/****************** Variaveis Globais ***********************/

int buffer[BUFFER_SIZE]; /* Buffer */
int buffer_index;
int num_items = 0;
pthread_mutex_t buffer_mutex; /* Mutex do buffer */
sem_t buffer_cheio;  /* Quando for 0, o buffer está cheio. */
sem_t buffer_vazio; /* Quando for 0, o buffer está vazio (funciona como um índice). */

// Display
Display display(BlackLib::GPIO_65, BlackLib::GPIO_45,
					BlackLib::GPIO_69, BlackLib::GPIO_60, BlackLib::GPIO_27, BlackLib::GPIO_66,
					BlackLib::GPIO_49, BlackLib::GPIO_115,
					BlackLib::GPIO_20, BlackLib::GPIO_47,
					BlackLib::GPIO_48, BlackLib::GPIO_46);
					

void *thread_consumidor(void *arg);
void *thread_produtor(void *arg);
void resetar_buffer(void);

/****************** FIM de Globais ***********************/

int main()
{	
    int res;
    pthread_t consumdior, produtor;
    // Botão de reset.
    BlackLib::BlackGPIO entrada(BlackLib::GPIO_68, BlackLib::input, BlackLib::SecureMode);	
    

    resetar_buffer();
    display.showNumber(0);
	
    while(1)
    {
	printf("Programa principal criando thread consumidor...\n");
	res = pthread_create(&consumdior, NULL, thread_consumidor, (void *) 1);
	if (res != 0)
	{
	    perror("A Craição da Thread consumidor falhou");
	    exit(EXIT_FAILURE);
	}


	printf("Programa principal criando thread produtor...\n");
	res = pthread_create(&produtor, NULL, thread_produtor, (void *) 2);
	if (res != 0)
	{
	    perror("A Craição da Thread produtor falhou");
	    exit(EXIT_FAILURE);
	}
	
	// Aguarda o reset.
	while(entrada.getValue() == "1");

	// Cancela as threads.
	pthread_cancel(consumdior);
	pthread_cancel(produtor);
	
	resetar_buffer();
	display.showNumber(0);
	sleep(1);
    }
}

void *thread_produtor(void *valor)
{
    int prevType;
    float perc1;
    ADC pot1(AIN0); // pot1 = consumidor
    
    // A thread pode ser cancelada de forma assíncrona.
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &prevType);
    
    while (1)
    {
	//sem_getvalue(&buffer_cheio, &val);
	//printf("Semáforo do produtor: %d\n", val);
	    
	perc1 = pot1.getPercentValue();	
	usleep(perc1 * 100 * UNIT_MS); // Espera muito ou pouco
	    
	// Se o buffer está cheio, espere (decrementa 1 do buffer cheio). 
	sem_wait(&buffer_cheio);
	
	// Trava o buffer.
	pthread_mutex_lock(&buffer_mutex);
		
	// Insere o item no buffer.
	buffer[buffer_index] = 1;
	num_items++;
	
	if(buffer_index < BUFFER_SIZE-1)
	{
	    buffer_index += 1;
	}
	
	//printf("Produzindo item %d ...\n", buffer_index);
	//fflush(stdout);
	display.showNumber(num_items);
	
	// Libera o buffer.
	pthread_mutex_unlock(&buffer_mutex);
	
	// Incrementa o semáforo (diz que está menos vazio).
	sem_post(&buffer_vazio);
		    
	usleep(100 * UNIT_MS);
    }

}

void *thread_consumidor(void *valor)
{
    int prevType;
    float perc2;
    ADC pot2(AIN1); // pot2 = produtor
    
    // A thread pode ser cancelada de forma assíncrona.
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &prevType);
    
    //int val;
    while (1)
    {
	//sem_getvalue(&buffer_vazio, &val);
	//printf("Semáforo do consumidor: %d\n", val);
	    
	perc2 = pot2.getPercentValue();
	usleep(perc2  * 100 *  UNIT_MS); // Espera muito ou pouco

	// Se o buffer está vazio, espere (decrementa 1 do buffer vazio).
	sem_wait(&buffer_vazio);
    
	// Trava o buffer.
	pthread_mutex_lock(&buffer_mutex);
	
	// Insere o item no buffer.
	buffer[buffer_index] = 0;
	num_items--;
	if (buffer_index > 0)
	{
	    buffer_index -= 1;
	}
	
	//printf("Consumindo item %d ...\n", buffer_index);
	display.showNumber(num_items);
	
	// Libera o buffer.
	pthread_mutex_unlock(&buffer_mutex);
	
	// Incrementa o semáforo (diz que está menos cheio).
	sem_post(&buffer_cheio);
			
	usleep(100 * UNIT_MS);
	    
    }
}

void resetar_buffer(void)
{
    int i;
    buffer_index = 0;
    num_items = 0;
    
    sem_init(&buffer_cheio, 0, BUFFER_SIZE);
    sem_init(&buffer_vazio, 0, 0);
    pthread_mutex_init(&buffer_mutex, NULL);
    
    for(i = 0; i < BUFFER_SIZE; i++)
    {
	buffer[i] = 0;
    }
}


