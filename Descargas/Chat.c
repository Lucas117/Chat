/*
 ============================================================================
 Name        : Chat.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#define MYPORT 3490
#define BACKLOG 10 //conexiones en cola
#define MAXDATASIZE 100

struct Sockaddr
{
	unsigned short sa_family;
	char sa_data[14];
};

struct In_addr
{
	unsigned long s_addr;
};

struct Sockaddr_in
{
	short int sin_family;
	unsigned short int sin_port;
	struct In_addr sin_addr;
	unsigned char sin_zero[8];

};

struct Hostent
{
	char *h_name;
	char **h_aliases;
	int h_addrtype;
	int h_lenght;
	char **h_addr_list;
};
#define h_addr h_addr_list[0];


int main(void) {
	int sockfd, new_fd, numBytes;
	struct Sockaddr_in my_addr;
	struct Sockaddr_in client_addr;
	char bufer[MAXDATASIZE];
	int mjsEnviado = 1;
	int msjRecibido = 1;
	int yes=1;
	int sin_size;
	char *chat_dialogo;
	chat_dialogo = (char *) malloc(100);
	int long_chat_dialogo;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}
	if((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) == -1)
	{
		perror("setsockopt");
		exit(1);
	}

	my_addr.sin_family = AF_INET; // ordenacion de byte de la maquina
	my_addr.sin_port = htons(MYPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(my_addr.sin_zero), '/0', 8);
	if((bind(sockfd, (struct Sockaddr *)&my_addr, sizeof(struct Sockaddr))) == -1)
	{
		perror("bind");
		exit(1);
	}
	if((listen(sockfd, BACKLOG)) == -1 )
	{
		perror("listen");
		exit(1);
	}

	while(1)
	{
		sin_size = sizeof(struct Sockaddr_in);
		if((new_fd = accept(sockfd, (struct Sockaddr *)&client_addr, &sin_size)) == -1 )
		{
			perror("accept");
			continue;
		}
		printf("Conexion establecida");
		//printf("Servidor: Conexion establecida con %s/n", inet_ntoa(client_addr.sin_addr));

		if(!fork()) //proceso hijo
		{
			//Mensaje servidor
			printf("Servidor: ");
			scanf("%*c%[^\n]", chat_dialogo);
			long_chat_dialogo = strlen(chat_dialogo);
			do
			{
			if((send(new_fd, chat_dialogo, long_chat_dialogo, 0)) == -1 )
				perror("send");
			else
				mjsEnviado = 0;
			}while(mjsEnviado);
			if((strcmp(chat_dialogo, "desconectar")) == 0 )
				break;
			//Mensaje Cliente
			do
			{
			if((numBytes=recv(sockfd, bufer, MAXDATASIZE-1, 0)) == -1)
				perror("recv");
			else
				msjRecibido = 0;
			}while(msjRecibido);
			bufer[numBytes] = '/0';
			if((strcmp(bufer, "desconectar")) == 0 )
				break;
			printf("Cliente: %s/n", bufer);
			continue;
		}
		break;
	}
	close(sockfd);
	close(new_fd);
	return 0;
}
