#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h> // biblioteca pthread
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "adc.h"
#include "BlackTime/BlackTime.h"
#include "BlackGPIO/BlackGPIO.h"
#include "ADC/Adc.h"
#include "display.h"

#define UNIT_MS 1000
#define UNIT_SEC 1000000

#define MULTICAST_ADDR "225.0.0.37"

#define PORTA_ENVIO 9706 // Grupo 6
#define PORTA_RECEBIMENTO_BASE 9800


using namespace std;

/****************** Variaveis Globais ***********************/

// Da esquerda para direita.
BlackLib::BlackGPIO e4(BlackLib::GPIO_26, BlackLib::input, BlackLib::SecureMode);	
BlackLib::BlackGPIO e3(BlackLib::GPIO_44, BlackLib::input, BlackLib::SecureMode);
BlackLib::BlackGPIO e2(BlackLib::GPIO_68, BlackLib::input, BlackLib::SecureMode);
BlackLib::BlackGPIO e1(BlackLib::GPIO_67, BlackLib::input, BlackLib::SecureMode);

// Display
Display display(BlackLib::GPIO_65, BlackLib::GPIO_45,
					BlackLib::GPIO_69, BlackLib::GPIO_60, BlackLib::GPIO_27, BlackLib::GPIO_66,
					BlackLib::GPIO_49, BlackLib::GPIO_115,
					BlackLib::GPIO_20, BlackLib::GPIO_47,
					BlackLib::GPIO_48, BlackLib::GPIO_46);
                    

void *enviar(void *arg);
void *receber(void *arg);

/****************** FIM de Globais ***********************/

int main()
{    
    int res;
    pthread_t thread_envio, thread_recebimento;
    void *thread_result;
        
    display.showNumber(0);
    
    printf("Programa principal criando thread de envio...\n");
    res = pthread_create(&thread_envio, NULL, enviar, (void *) 1);
    if (res != 0)
    {
        perror("A Craição da thread de envio falhou");
        exit(EXIT_FAILURE);
    }


    printf("Programa principal criando thread produtor...\n");
    res = pthread_create(&thread_recebimento, NULL, receber, (void *) 2);
    if (res != 0)
    {
        perror("A Craição da thread de recebimento falhou");
        exit(EXIT_FAILURE);
    }
    
    res = pthread_join(thread_envio, &thread_result);
    res = pthread_join(thread_recebimento, &thread_result);

}

void *enviar(void *valor)
{
    ADC pot0(AIN0), pot1(AIN1);
    float buffer[2];
    
    // Necessários para o multicast
    int sockfd, len;
    struct sockaddr_in address;
    
    unsigned short porta = PORTA_ENVIO;
    
    sockfd  = socket(AF_INET, SOCK_DGRAM,0);  // criacao do socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    //address.sin_addr.s_addr = inet_addr("10.13.96.16"); // euler
    address.sin_port = htons(porta);
    
    len = sizeof(address);
    
    while (1)
    {
        
        buffer[0] = pot0.getPercentValue() / 100.0;    
        buffer[1] = pot1.getPercentValue() / 100.0;    

        printf("[thread 0] Enviando %f e %f para porta %d...\n", buffer[0], buffer[1], porta);

        sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &address, len);
        usleep(1000 * UNIT_MS);
    }
}


void *receber(void *valor)
{

    bool buffer[8];
    string numeroBin;
    int numeroInt;


    int server_sockfd;
    size_t server_len;
    socklen_t client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    unsigned short porta;
    struct ip_mreq mreq;  // para endereço multicast
    
    
    if ( (server_sockfd = socket(AF_INET, SOCK_DGRAM, 0) )  < 0  )  // cria um novo socket
    {
        printf("[thread 1] Houve erro na ebertura do socket ");
        exit(1);
    }
    
    numeroBin = e1.getValue() + e2.getValue() + e3.getValue() + e4.getValue(); // Cria o número binário.
    numeroInt = stoi(numeroBin, nullptr, 2); // Converte a string (em binário) para inteiro

    porta = PORTA_RECEBIMENTO_BASE + numeroInt;

    unlink("server_socket");  // remocao de socket antigo
    memset(&server_address, 0, sizeof(server_address));


    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(porta);

    server_len = sizeof(server_address);

    if(bind(server_sockfd, (struct sockaddr *) &server_address, server_len) < 0 )
    {
        perror("[thread 1] Houve error no Bind");
        exit(1);
    }

    // use setsockopt() para requerer inscrição num grupo multicast
    mreq.imr_multiaddr.s_addr=inet_addr(MULTICAST_ADDR);
    mreq.imr_interface.s_addr=htonl(INADDR_ANY);
    if (setsockopt(server_sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq,sizeof(mreq)) < 0) 
    {
        perror("[thread 1] setsockopt");
        exit(1);
    }

    //printf(" IPPROTO_IP = %d\n", IPPROTO_IP);
    //printf(" SOL_SOCKET = %d\n", SOL_SOCKET);
    //printf(" IP_ADD_MEMBERSHIP = %d \n", IP_ADD_MEMBERSHIP);
    
    client_len = sizeof(client_address);
    
    while (1)
    {
        
        
        printf("[thread 1] Servidor esperando na porta %d ...\n", porta);
        
        
        if(recvfrom(server_sockfd, buffer, sizeof(buffer), 0,
                    (struct sockaddr *) &client_address, &client_len) < 0 )
        {
            perror(" erro no RECVFROM( )");
            exit(1);
        }
        
        printf("Valor recebido na porta %d foi: %d:%d:%d:%d:%d:%d:%d:%d\n", porta, buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7]);
        
        // Neste ponto já se tem o vetor de booleanos preenchido.
        display.ligarSegmentos(buffer);

        usleep(1000 * UNIT_MS);
    }

}
