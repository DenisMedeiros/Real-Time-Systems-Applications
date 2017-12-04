//
//  servidor_multicast.cpp
//  str
//
//  Created by Affonso on 18/05/16.
//  Copyright © 2016 Affonso. All rights reserved.
//

// #include "servidor_multicast.hpp"

// programa servidor_multicast.cpp - Servidor que lê e escreve em um vetor de caracter
// Protocolo UDP

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


#define MULTICAST_ADDR "225.0.0.37"
void *thread_enviar(void *valor);
void *thread_receber(void *valor);
pthread_t servidor, cliente;
int server_sockfd, client_sockfd;
size_t server_len;
socklen_t client_len;
struct sockaddr_in server_address;
struct sockaddr_in client_address;

struct ip_mreq mreq;  // para endere�o multicast
int valor;
unsigned short porta = 9709;


int main( )
{
    int res;

    void *thread_result;
	printf("Programa principal criando thread servidor...\n");
	res = pthread_create(&servidor, NULL, thread_receber, (void *) 1);
	if (res != 0)
	{
	    perror("A Craição da Thread servidor falhou");
	    exit(EXIT_FAILURE);
	}

	printf("Programa principal criando thread cliente...\n");
	res = pthread_create(&cliente, NULL, thread_enviar, (void *) 2);
	if (res != 0)
	{
	    perror("A Craição da Thread cliente falhou");
	    exit(EXIT_FAILURE);
	}
	
    printf("\nMAIN()--> Aguardando a THREAD terminar servidor...\n");
    res = pthread_join(servidor, &thread_result);
    if (res != 0) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }

     printf("\nMAIN()--> Aguardando a THREAD terminar cliente...\n");
    res = pthread_join(cliente, &thread_result);
    if (res != 0) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }

    printf("MAIN() --> TODAS AS THREADS terminaram\n");

    exit(EXIT_SUCCESS);     
}

void *thread_receber(void *valor)
{
    unlink("server_socket");  // remocao de socket antigo
    if ( (server_sockfd = socket(AF_INET, SOCK_DGRAM, 0) )  < 0  )  // cria um novo socket
    {
        printf(" Houve erro na ebertura do socket ");
        exit(1);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(porta);
    
    server_len = sizeof(server_address);
    
    if(  bind(server_sockfd, (struct sockaddr *) &server_address, server_len) < 0 )
    {
        perror("Houve error no Bind");
        exit(1);
    }
    
    mreq.imr_multiaddr.s_addr=inet_addr(MULTICAST_ADDR);
    mreq.imr_interface.s_addr=htonl(INADDR_ANY);
    if (setsockopt(server_sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
        perror("setsockopt");
        exit(1);
    }
    //printf(" IPPROTO_IP = %d\n", IPPROTO_IP);
    //printf(" SOL_SOCKET = %d\n", SOL_SOCKET);
    //printf(" IP_ADD_MEMBERSHIP = %d \n", IP_ADD_MEMBERSHIP);

    float buffer_recebe[2];
    while(1)
    {
        printf("Servidor esperando ...\n"k);
        
        client_len = sizeof(server_address);
        if(recvfrom(server_sockfd, &buffer_recebe, sizeof(buffer_recebe),0,(struct sockaddr *) &server_address, &client_len) < 0 )
        {
            perror(" erro no RECVFROM( )");
            exit(1);
        }
        printf(" Valor recebido foi: buffer_recebe[0]: %f buffer_recebe[1] %f\n", buffer_recebe[0],buffer_recebe[1]);
    }
}

void *thread_enviar(void *valor)
{
    client_sockfd  = socket(AF_INET, SOCK_DGRAM,0);  // criacao do socket
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    client_address.sin_port = htons(porta);
    client_len = sizeof(client_address);
    bool buffer_envia[8];
    while(1)
    {
        buffer_envia[0] = rand() % 2;
        buffer_envia[1] = rand() % 2;
        buffer_envia[2] = rand() % 2;
        buffer_envia[3] = rand() % 2;
        buffer_envia[4] = rand() % 2;
        buffer_envia[5] = rand() % 2;
        buffer_envia[6] = rand() % 2;
        buffer_envia[7] = rand() % 2;
        printf(" Valor enviado foi: %d:%d:%d:%d:%d:%d:%d:%d\n", buffer_envia[0],buffer_envia[1],buffer_envia[2],buffer_envia[3],buffer_envia[4],buffer_envia[5],buffer_envia[6],buffer_envia[7]);
        sendto(client_sockfd, &buffer_envia,sizeof(buffer_envia),0,(struct sockaddr *) &client_address, client_len);
        sleep(1);
    }
}