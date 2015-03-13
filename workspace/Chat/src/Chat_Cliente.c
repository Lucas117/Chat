/*
 ============================================================================
 Name        : Chat_Cliente.c
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
#define SERVERPORT 3490
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

int main(int argc, char *argv[]) {
	int sockfd, numBytes;
	char bufer[MAXDATASIZE];
	char ip_server[20];
	int mjsEnviado = 1;
	int msjRecibido = 1;
	char *chat_dialogo;
	chat_dialogo = (char *) malloc(100);
	int long_chat_dialogo;
	//struct Hostent *he;
	struct Sockaddr_in server_addr;
	printf("IP servidor: ");
	scanf("%s/n", ip_server);
	/*if(argc !=2)
	{
		fprintf(stderr, "Usar direccion IP del servidor /n");
		exit(1);
	}
	if((he = gethostbyname(argv[1])) == NULL)
	{
		perror("gethostbyname");
		exit(1);
	}
	*/
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		perror("socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVERPORT);
	server_addr.sin_addr.s_addr = inet_addr(ip_server);
	//server_addr.sin_addr = *((struct In_addr *)he->h_addr_list);
	memset (&(server_addr.sin_zero), '/0', 8);
	if((connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))) == -1)
	{
		perror("connect");
		exit(1);
	}
	if((numBytes=recv(sockfd, bufer, MAXDATASIZE - 1, 0)) == -1 )
	{
		perror("recv");
		exit(1);
	}
	bufer[numBytes] = '/0';
	printf("Servidor: %s/n", bufer);
	while(1)
	{
		//Mensaje cliente
		printf("Cliente: ");
		scanf("%*c%[^\n]", chat_dialogo);
		long_chat_dialogo = strlen(chat_dialogo);
		do
		{
			if((send(sockfd, chat_dialogo, long_chat_dialogo, 0)) == -1 )
				perror("send");
			else
				mjsEnviado = 0;
		}while(mjsEnviado);
		if((strcmp(chat_dialogo, "desconectar")) == 0 )
			break;
		//Mensaje servidor
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
		printf("Servidor: %s/n", bufer);
	}
	close(sockfd);
	return 0;
}
