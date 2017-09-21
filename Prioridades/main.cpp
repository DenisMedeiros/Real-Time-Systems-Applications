#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <cmath>
#include <sys/types.h>
#include "BlackGPIO/BlackGPIO.h"
#include "adc.h"

#define UNIT_MS 1000
#define UNIT_SEC 1000000

using namespace std;

/* Faz várias operações matemáticas só para gastar tempo. */
double carga()
{
    srand(time(NULL));
    double a = 1.293892838761287367813678123;
    
    for(int i = 0; i < 9999; i++)
    {
        a *= 0.00030027 * (rand() % 3331) + 0.010309278;
        a -= pow(a, 0.987654321);
        a += log10((rand() % 101) + 1) + log((rand() % 107) + 1);
        a /= 3 + sin(rand() % 7*i) + cos(rand() % (11 + i));
    }
    return a;
}

int main(int argc, char * argv[])
{   
    /* Usado para o compilador não otimizar a função de carga. */
    double lixo;
    
    /* PIDs dos processos filhos que este processo controlará a prioridade. */
    pid_t pid1, pid2;

    /* Cria as saídas digitaos dos dois LEDs. */
    BlackLib::BlackGPIO led1(BlackLib::GPIO_67, BlackLib::output, BlackLib::SecureMode);
    BlackLib::BlackGPIO led2(BlackLib::GPIO_68, BlackLib::output, BlackLib::SecureMode);
    
    /* Cria as duas entradas analógicas para os potenciômetros. */
    ADC pot1(AIN0);
    ADC pot2(AIN1);
    
    if((pid1 = fork()) == 0)
    {
        cout << "Sou filho 1 e tenho pid " << getpid() << endl;
        while(1);
    }
    else if((pid2 = fork()) == 0)
    {
        cout << "Sou filho 2 e tenho pid " << getpid() << endl;
        while(1);
    }
    else
    {
        cout << "Sou pai de " << pid1 << " e " << pid2 << endl;
        while(1);
    }
    
    /*
    while(1)
    {
        lixo = carga();
        led1.setValue(BlackLib::low);
        led2.setValue(BlackLib::low);
        lixo = carga();
        led1.setValue(BlackLib::high);
        led2.setValue(BlackLib::high);
        //cout << "pot1 = " << pot1.getPercentValue() << endl;
        //cout << "pot2 = " << pot2.getPercentValue() << endl;
    }
    */ 
    
    cout << lixo << endl;
    return 0;
}
