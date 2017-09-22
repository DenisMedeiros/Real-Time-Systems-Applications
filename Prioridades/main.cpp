#include <iostream>
#include <cmath>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/resource.h>
#include "BlackGPIO/BlackGPIO.h"
#include "adc.h"

#define UNIT_MS 1000
#define UNIT_SEC 1000000

#define PRIOR_MUITO_ALTA 0
#define PRIOR_ALTA 5
#define PRIOR_BAIXA 15

using namespace std;

/* Faz várias operações matemáticas só para gastar tempo. */
void carga()
{
    srand(time(NULL));
    double a = 1.293892838761287367813678123;
    
    for(int i = 0; i < 50000; i++)
    {
        a *= 0.00030027 * (rand() % 3331) + 0.010309278;
        a -= pow(a, 0.987654321);
        a += log10((rand() % 101) + 1) + log((rand() % 107) + 1);
        a /= 3 + sin(rand() % 7*i) + cos(rand() % (11 + i));
    }
}

int main(int argc, char * argv[])
{   
    /* PIDs dos processos filhos que este processo controlará a prioridade. */
    pid_t pid1, pid2;
    

    if((pid1 = fork()) == 0) /* Filho 1 */
    {
        /* Cria a saíd digital para o LED 2. */
        BlackLib::BlackGPIO led1(BlackLib::GPIO_67, BlackLib::output, BlackLib::SecureMode);
        while(1)
        {
            led1.setValue(BlackLib::low);
            carga();
            led1.setValue(BlackLib::high);
            carga();
        }
    }
    else if((pid2 = fork()) == 0) /* Filho 2 */
    {
        /* Cria a saíd digital para o LED 2. */
        BlackLib::BlackGPIO led2(BlackLib::GPIO_68, BlackLib::output, BlackLib::SecureMode);

        while(1)
        {
            led2.setValue(BlackLib::low);
            carga();
            led2.setValue(BlackLib::high);
            carga();
        }
    }
    else /* Pai */
    {
        /* Cria as duas entradas analógicas para os potenciômetros. */
        ADC pot1(AIN0);
        ADC pot2(AIN1);
        
        float perc1;
        float perc2;
     
        /* Processo pai deve ter prioridade maior que os filhos. */
        setpriority(PRIO_PROCESS, 0, 0);
        setpriority(PRIO_PROCESS, pid1, 5);
        setpriority(PRIO_PROCESS, pid2, 5);
        
        while(1)
        {
            /* Aguarda 500 ms */
            usleep(500 * UNIT_MS);
            
            /* Obtém os valores percentuais do ADC */
            perc1 = pot1.getPercentValue();
            perc2 = pot2.getPercentValue();
            
            
            /* Altera a prioridade de ambos os processos 55.56% é (1/1.8V). */
            
            if(perc1 > 55.56) 
            {
                setpriority(PRIO_PROCESS, pid1, 5);
            }
            else
            {
                setpriority(PRIO_PROCESS, pid1, 19);
            }
            
            if(perc2 > 55.56) // Equivale a 1V (1/1.8V)
            {
                setpriority(PRIO_PROCESS, pid2, 5);
            }
            else
            {
                setpriority(PRIO_PROCESS, pid2, 19);
            }
            
            /* Exibe as informações úteis do sistema. */
            cout << "---------------------------------------------" << endl;
            cout << "Valor do potenciômetro 1: " << perc1 << endl;
            cout << "Valor do potenciômetro 2: " << perc2 << endl;
            cout << "Prioridade do pai: " << getpriority(PRIO_PROCESS, 0) << endl;
            cout << "Prioridade do filho 1: " << getpriority(PRIO_PROCESS, pid1) << endl;
            cout << "Prioridade do filho 2: " << getpriority(PRIO_PROCESS, pid2) << endl;
            cout << "---------------------------------------------" << endl;
            
        }
    }
    

    return 0;
}
