#include<stdio.h>
#include<netinet/in.h>			//for Internet address family, defines sockaddr_in
#include<sys/types.h>			//for p_id
#include<sys/socket.h>
#include<netdb.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<ctype.h>

#define MAXLINE 4096
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
char buff[MAXLINE];
int flag = 0;

void displayMenu()
{
	printf("\nUse the following messages to communicate with server :\n"); 
	printf("1. register - To register with server\n");
	printf("2. login - To communicate with server (if client is already registered)\n");
	printf("3. sound - To confirm that connection has established\n");
	printf("4. store - To store new animal-voice pair\n");
	printf("5. query - To get names of all stored animals\n");
	printf("6. bye   - To instruct server that client no longer needs this session\n");
	printf("7. end   - To instruct server to shut down\n");
	printf("8. animal name - To get voice of given animal\n");
	printf("\nEnter message....\n");
}
void str_cli(int sockfd)
{
	int maxfd;			//max number of descriptors
	fd_set rset;
	int n;
	int stdineof = 0;		//to handle EOF correctly
	FD_ZERO(&rset);
	for(;;)
	{
		if(stdineof == 0)
			FD_SET(fileno(stdin) , &rset);
		FD_SET(sockfd , &rset);
		maxfd = MAX(fileno(stdin) , sockfd)+1;
		select(maxfd , &rset , NULL , NULL , NULL);
		if(FD_ISSET(sockfd , &rset))
		{
			//socket is readable
			if((n = read(sockfd , buff , MAXLINE)) == 0)
			{
				if(stdineof == 1)
				   return;
				else
				   perror("Server terminated prematurely!!!");
				exit(0);
			}
			printf("%s",buff);
			
		}
		if(FD_ISSET(fileno(stdin) , &rset))
		{
			//input is readable
			
			if((n = read(fileno(stdin),buff, MAXLINE)) == 0)		//EOF 
			{
			   stdineof = 1;
			   shutdown(sockfd ,1);    //SHUT_WR
			   FD_CLR(fileno(stdin),&rset);
			   continue;
			} 
			for(int i = 0;i<n;i++)
				buff[i] = toupper(buff[i]);
			if(strncmp(buff,"REGISTER\n",n) == 0 || strncmp(buff,"LOGIN\n",n) == 0)
				flag = 1;
			if(flag)	
				write(sockfd ,buff,n);
			else
			{
				printf("First register yourself !!\n");
				displayMenu();
			}
		}
	}
}
int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;
	if (argc != 2)
		perror("usage: <IPaddress>");
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket error");
		exit(0);
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5035);
	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <=0 )
	{
		perror("inet_pton error for %s");
		exit(0);
	}
	printf("********TCP Client-Server Application*********\n");
	printf("Server returns the sounds made by animals requested by the client\n");
	printf("******************TCP Client******************\n");
	if(connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
	{
        	perror("connect error");
		exit(0);
	}
        
	displayMenu();
	str_cli(sockfd); 
	close(sockfd);
	return 0;
}



