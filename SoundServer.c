#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<ctype.h>

#define BACKLOG 5
#define MAXLINE 4096
#define EINTR 3407
#define SERV_PORT 5035

char buff[MAXLINE];
int errorno;
int SIZE = 3;
int USER_SIZE = 1;

char animal[10][30] = {"DOG","CAT","LION"};
char voice[10][30] = {"WOOF","MEOW","ROAR"};

char username[10][30] = {"ADMIN"};
char password[10][30] = {"1234"};
char** query(int* k)
{
	char** A;
        int noe = 2*SIZE + 4,i;
        A = (char**)malloc(noe * sizeof(char*));
        int x = 0;
	for(int j = 0;j<noe;j++)
	    A[j] = (char*)malloc(30 * sizeof(char));
	strcpy(A[x++],"Server : ");
        for(int i = 0;i < SIZE;i++)
	{
	    strcpy(A[x++],animal[i]);
	    strcpy(A[x++],"\n");
        }
	strcat(A[x++],"QUERY:OK");
	strcat(A[x++],"\n");
	strcat(A[x++],"\nEnter message....\n");
        *k = x;
	return A;
} 
char* search(char Animal[])
{
	char *A = (char*)malloc(sizeof(char)*80);
	for(int i = 0;i<SIZE;i++)
	{
		if(strcmp(animal[i],Animal) == 0)
		{
			strcpy(A,"Server : A ");strcat(A,animal[i]);strcat(A," SAYS ");
			strcat(A,voice[i]);
			strcat(A,"\n");
			strcat(A,"\nEnter message....\n");
			return A;
		}
    }
    strcpy(A,"Server : I DON'T KNOW ");
    strcat(A,Animal);
    strcat(A,"\n");
    strcat(A,"\nEnter message....\n");
    return A;
}
void store(char Animal[] , char Voice[])
{
	if(SIZE > 10)
		return;
	for(int i = 0;i<SIZE;i++)
	{
		if(strcmp(animal[i],Animal) == 0)
		{
			strcpy(voice[i] ,Voice);
			return;
		}
	}
	strcpy(animal[SIZE], Animal);
	strcpy(voice[SIZE] ,Voice);
	SIZE++;
}

int authenticate(char Username[], char Password[])
{
	for(int i = 0;i<USER_SIZE;i++)
	{
		if(strcmp(username[i],Username) == 0)
		{
			if(strcmp(password[i],Password) == 0)
			{
				return 1;
			}
			else
				return 0;
		}
    	}
    	return 0;
}

void registerUser(char Username[], char Password[])
{
	strcpy(username[USER_SIZE], Username);
	strcpy(password[USER_SIZE], Password);
	USER_SIZE++;
}
	
int main(int argc , char** argv)
{
   int i , maxi ,maxfd ,listenfd, connfd,sockfd;
   int nready ,n,client[FD_SETSIZE];
   fd_set rset,allset;
   
   socklen_t clilen;  									//length of client socket structure
   struct sockaddr_in servaddr,cli_addr;
   
   listenfd = socket(AF_INET, SOCK_STREAM, 0);
   bzero(&servaddr, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   		//Foreign IP address
   servaddr.sin_port = htons(SERV_PORT);             		//Foreign Port Number
   printf("********TCP Client-Server Application*********\n");
   printf("******************TCP SERVER******************\n");
   
   bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
   printf("\nListening on port %d \n",SERV_PORT);
   listen(listenfd, 10);
   printf("\nWaiting for connections...\n");
   maxfd = listenfd;			//descriptor of listening socket
   maxi = -1;
   for(int i = 0;i<FD_SETSIZE;i++)
   	client[i] = -1;
   FD_ZERO(&allset);
   FD_SET(listenfd , &allset);
  
   for(; ;)
    {
       rset = allset;
       nready = select(maxfd+1 ,&rset,NULL,NULL,NULL);
       
       //new client connection
       if(FD_ISSET(listenfd , &rset))
       {
	   clilen = sizeof(cli_addr);
           if((connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen)) <0 )		//checking for client
	   {
		if(errorno == EINTR)
		   continue;
	   	else
	   	{
			perror("\nNot connected\n");
			break;
	   	}
	   }
           for(i = 0;i<FD_SETSIZE;i++)
               if(client[i] < 0)
               {
		   client[i] = connfd;	
		    break;
		}
            if(i == FD_SETSIZE)
	    {
		perror("\nToo many clients...");
		exit(0);
	     }
	    
            FD_SET(connfd,&allset);
            if(connfd > maxfd)
		maxfd = connfd;
            if(i > maxi)
		maxi = i;
            if(--nready <= 0)
		continue;
         }
	//check all clients for data
        for(i = 0;i<= maxi;i++)
	{
	    if((sockfd = client[i]) < 0)
		continue;
            if(FD_ISSET(sockfd , &rset))
            {
		int flag = 0;
		if((n = read(sockfd , buff , MAXLINE)) == 0)
		{
			//connection closed by client
			close(sockfd);
			FD_CLR(sockfd , &allset);
			client[i] = -1;
		}
		else
		{
			
			if(strncmp(buff,"REGISTER\n",n) == 0)
			 {
			     strcpy(buff,"Enter Username :\n");
			     write(sockfd,buff,MAXLINE);
			     n = read(sockfd , buff , MAXLINE);
			     char U[n];
 			     strncpy(U,buff,n-1);U[n-1] = '\0';
			     strcpy(buff,"Enter password :\n");
			     write(sockfd,buff,MAXLINE);
			     n = read(sockfd , buff , MAXLINE);
			     char P[n];
			     strncpy(P,buff,n-1);P[n-1] = '\0';
			     registerUser(U,P);
			     //printf("1\n");
			     strcpy(buff,"Connected to server ...\n\nEnter message....\n");
			     flag = 1;
			  }
			  else if(strncmp(buff , "LOGIN\n",n) == 0)
			   {
				strcpy(buff,"Enter Username :\n");
			        write(sockfd,buff,MAXLINE);
			     	n = read(sockfd , buff , MAXLINE);
			     	char U[n];
 			     	strncpy(U,buff,n-1);U[n-1] = '\0';
				strcpy(buff,"Enter password :\n");
			     	write(sockfd,buff,MAXLINE);
			     	n = read(sockfd , buff , MAXLINE);
			     	char P[n];
			     	strncpy(P,buff,n-1);P[n-1] = '\0';
				
			     	int c = authenticate(U,P);
			     	if(c == 1)
					strcpy(buff,"Connected to server ...\n");
			        else
			          strcpy(buff,"Server : Sorry your id and password is not correct:(\nEnter register/login to connect to server ...\n\nEnter message....\n");
			    }
		
			else if(strncmp(buff , "SOUND\n",n)== 0)
			 {
			   printf("\nConnected to Client");
			   printf("%d",maxi);
			   printf("\n");
			   strcpy(buff ,"Server : SOUND:OK\n\nEnter message....\n");
			}
			else if(strncmp(buff,"QUERY\n",n)== 0)
			{
			   int l = 0;
			   char** q = query(&l);
			   char one[MAXLINE];
			   //one[0] = "";
			   for(int i = 0;i<l;i++)
			   {	
				strcat(one,q[i]);
			    }
			    strcpy(buff,one);
			    one[0] = '\0';
			}
			else if(strncmp(buff , "STORE\n",n) == 0)
			{
			     strcpy(buff,"Enter Animal :\n");
			     write(sockfd,buff,MAXLINE);
			     n = read(sockfd , buff , MAXLINE);
			     char A[n];
 			     strncpy(A,buff,n-1);A[n-1] = '\0';
			     strcpy(buff,"Enter Voice :\n");
			     write(sockfd,buff,MAXLINE);
			     n = read(sockfd , buff , MAXLINE);
			     char V[n-1];
			     strncpy(V,buff,n-1);V[n-1] = '\0';
			     store(A,V);
			     strcpy(buff,"Server : STORE:OK\n\nEnter message....\n");
			}
			else if(strncmp(buff,"BYE\n",n) == 0)
			{
			   printf("\nClient");
			   printf("%d",i);
			   printf("is disconnected..\n");
			   strcpy(buff,"Server : BYE:OK\n");
			   write(sockfd,buff,MAXLINE);
			   close(sockfd);
			   FD_CLR(sockfd , &allset);
			   client[i] = -1;
			   }
			else if(strncmp(buff , "END\n",n) == 0)
			{
			   strcpy(buff,"Server : END:OK\n");
			   write(sockfd,buff,MAXLINE);
			   exit(0);
			 }
			else
			{
			   char extr[n];
			   strncpy(extr,buff,n-1);extr[n-1] = '\0';
			   strcpy(buff,search(extr));
			   
			}
			write(sockfd,buff,MAXLINE);
			

			if(--nready <= 0)
			break;
		}
		
	     }
	 }
    }
    return 0;
}

