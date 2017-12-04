#ifndef DISPLAY_H_   /* Include guard */
#define DISPLAY_H_

#include "BlackGPIO/BlackGPIO.h"
#include <stdint.h>
#include <thread>
#include <mutex> 

class Display                   
{
    public:     
        Display(BlackLib::gpioName d1, BlackLib::gpioName d2, 
            BlackLib::gpioName d3, BlackLib::gpioName d4, 
            BlackLib::gpioName sA, BlackLib::gpioName sB, 
            BlackLib::gpioName sC, BlackLib::gpioName sD, 
            BlackLib::gpioName sE, BlackLib::gpioName sF, 
            BlackLib::gpioName sG, BlackLib::gpioName sDP);
            
        ~Display();
            
        void showNumber(uint16_t _number);

    private:     
    
        /* Current number */
        uint16_t number;
        std::mutex mtx; 
    
        /* digits */
        BlackLib::BlackGPIO *digit1;
        BlackLib::BlackGPIO *digit2;
        BlackLib::BlackGPIO *digit3;
        BlackLib::BlackGPIO *digit4;
    
        /* 7 segmentns and double point */
        BlackLib::BlackGPIO *segA;
        BlackLib::BlackGPIO *segB;
        BlackLib::BlackGPIO *segC;
        BlackLib::BlackGPIO *segD;
        BlackLib::BlackGPIO *segE;
        BlackLib::BlackGPIO *segF;
        BlackLib::BlackGPIO *segG;
        BlackLib::BlackGPIO *segDP;
        
        void turnOnSegments(uint8_t number);
        void worker();
};

#endif // DISPLAY_H_
