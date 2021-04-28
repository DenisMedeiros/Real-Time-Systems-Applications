#include "display.h"
#include <stdexcept>
#include <unistd.h>

using namespace std;

Display::Display(BlackLib::gpioName d1, BlackLib::gpioName d2, 
            BlackLib::gpioName d3, BlackLib::gpioName d4, 
            BlackLib::gpioName sA, BlackLib::gpioName sB, 
            BlackLib::gpioName sC, BlackLib::gpioName sD, 
            BlackLib::gpioName sE, BlackLib::gpioName sF, 
            BlackLib::gpioName sG, BlackLib::gpioName sDP)
{
    digit1 = new BlackLib::BlackGPIO(d1, BlackLib::output, BlackLib::SecureMode); 
    digit2 = new BlackLib::BlackGPIO(d2, BlackLib::output, BlackLib::SecureMode);
    digit3 = new BlackLib::BlackGPIO(d3, BlackLib::output, BlackLib::SecureMode);
    digit4 = new BlackLib::BlackGPIO(d4, BlackLib::output, BlackLib::SecureMode);

    segA = new BlackLib::BlackGPIO(sA, BlackLib::output, BlackLib::SecureMode); 
    segB = new BlackLib::BlackGPIO(sB, BlackLib::output, BlackLib::SecureMode);
    segC = new BlackLib::BlackGPIO(sC, BlackLib::output, BlackLib::SecureMode);
    segD = new BlackLib::BlackGPIO(sD, BlackLib::output, BlackLib::SecureMode);
    segE = new BlackLib::BlackGPIO(sE, BlackLib::output, BlackLib::SecureMode); 
    segF = new BlackLib::BlackGPIO(sF, BlackLib::output, BlackLib::SecureMode);
    segG = new BlackLib::BlackGPIO(sG, BlackLib::output, BlackLib::SecureMode);
    segDP = new BlackLib::BlackGPIO(sDP, BlackLib::output, BlackLib::SecureMode);
    
    
    /* High select the digit */
    digit1->setValue(BlackLib::high);
    digit2->setValue(BlackLib::high);
    digit3->setValue(BlackLib::high);
    digit4->setValue(BlackLib::high);
    
    /* High voltage is off and low voltage is on */
    segA->setValue(BlackLib::high);
    segB->setValue(BlackLib::high);
    segC->setValue(BlackLib::high);
    segD->setValue(BlackLib::high);
    segE->setValue(BlackLib::high);
    segF->setValue(BlackLib::high);
    segG->setValue(BlackLib::high);
    segDP->setValue(BlackLib::high);
    
}

Display::~Display()
{

    /* High select the digit */
    digit1->setValue(BlackLib::low);
    digit2->setValue(BlackLib::low);
    digit3->setValue(BlackLib::low);
    digit4->setValue(BlackLib::low);
    
    /* High voltage is off and low voltage is on */
    segA->setValue(BlackLib::high);
    segB->setValue(BlackLib::high);
    segC->setValue(BlackLib::high);
    segD->setValue(BlackLib::high);
    segE->setValue(BlackLib::high);
    segF->setValue(BlackLib::high);
    segG->setValue(BlackLib::high);
    segDP->setValue(BlackLib::high);

    delete digit1;
    delete digit2;
    delete digit3;
    delete digit4;
    
    delete segA;
    delete segB;
    delete segC;
    delete segD;
    delete segE;
    delete segF;
    delete segG;
    delete segDP;
}

void Display::turnOnSegments(uint8_t number)
{ 

    switch(number)
    {
        case 0:

        segA->setValue(BlackLib::low);
        segB->setValue(BlackLib::low);
        segC->setValue(BlackLib::low);
        segD->setValue(BlackLib::low);
        segE->setValue(BlackLib::low);
        segF->setValue(BlackLib::low);
        
        break;
         
        case 1:
        
        segB->setValue(BlackLib::low);
        segC->setValue(BlackLib::low);
        break;
         
        case 2:
        
        segA->setValue(BlackLib::low);
        segB->setValue(BlackLib::low);
        segD->setValue(BlackLib::low);
        segE->setValue(BlackLib::low);
        segG->setValue(BlackLib::low);
         
        break;
         
        case 3:
         
        segA->setValue(BlackLib::low);
        segB->setValue(BlackLib::low);
        segC->setValue(BlackLib::low);
        segD->setValue(BlackLib::low);
        segG->setValue(BlackLib::low);
         
        break;
         
        case 4:
        
        segB->setValue(BlackLib::low);
        segC->setValue(BlackLib::low);
        segF->setValue(BlackLib::low);
        segG->setValue(BlackLib::low);
        break;
         
        case 5:
        
        segA->setValue(BlackLib::low);
        segC->setValue(BlackLib::low);
        segD->setValue(BlackLib::low);
        segF->setValue(BlackLib::low);
        segG->setValue(BlackLib::low);
        break;
         
        case 6:
        
        segA->setValue(BlackLib::low);
        segC->setValue(BlackLib::low);
        segD->setValue(BlackLib::low);
        segE->setValue(BlackLib::low);
        segF->setValue(BlackLib::low);
        segG->setValue(BlackLib::low);
        break;
         
        case 7:
        
        segA->setValue(BlackLib::low);
        segB->setValue(BlackLib::low);
        segC->setValue(BlackLib::low);
        break;
         
        case 8:
        
        segA->setValue(BlackLib::low);
        segB->setValue(BlackLib::low);
        segC->setValue(BlackLib::low);
        segD->setValue(BlackLib::low);
        segE->setValue(BlackLib::low);
        segF->setValue(BlackLib::low);
        segG->setValue(BlackLib::low);
        break;
         
        case 9:
        
        segA->setValue(BlackLib::low);
        segB->setValue(BlackLib::low);
        segC->setValue(BlackLib::low);
        segD->setValue(BlackLib::low);
        segF->setValue(BlackLib::low);
        segG->setValue(BlackLib::low);
        break;
         
        default:
        break;
    }
    

    
}

void Display::ligarSegmentos(bool segmentos[8])
{
    segA->setValue(BlackLib::high);
    segB->setValue(BlackLib::high);
    segC->setValue(BlackLib::high);
    segD->setValue(BlackLib::high);
    segE->setValue(BlackLib::high);
    segF->setValue(BlackLib::high);
    segG->setValue(BlackLib::high);
    segDP->setValue(BlackLib::high);

    if(segmentos[0]) segA->setValue(BlackLib::low);
    if(segmentos[1]) segB->setValue(BlackLib::low);
    if(segmentos[2]) segC->setValue(BlackLib::low);
    if(segmentos[3]) segD->setValue(BlackLib::low);
    if(segmentos[4]) segE->setValue(BlackLib::low);
    if(segmentos[5]) segF->setValue(BlackLib::low);
    if(segmentos[6]) segG->setValue(BlackLib::low);
    if(segmentos[7]) segDP->setValue(BlackLib::low);
    
}

void Display::showNumber(uint16_t _number)
{
    usleep(1000);
    segA->setValue(BlackLib::high);
    segB->setValue(BlackLib::high);
    segC->setValue(BlackLib::high);
    segD->setValue(BlackLib::high);
    segE->setValue(BlackLib::high);
    segF->setValue(BlackLib::high);
    segG->setValue(BlackLib::high);
    segDP->setValue(BlackLib::high);
    turnOnSegments(_number);  

}
