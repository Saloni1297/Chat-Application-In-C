// socket server example, handles multiple clients using threads

#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
#define SIZE 2000

//the thread function
void *connection_handler(void *);
char sendBuff[2000], client_message[2000];

void manipulating_string(char a[],int i)
{
	char d[50];
	memset(sendBuff,0,SIZE);
	strcpy(d,"From Client");
	sprintf(sendBuff,"%s%d : %s",d,i,a);
}

int socket1[10];
static int i;
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;

    server.sin_port = htons(atoi(argv[1]) );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 10);

    //Accept an incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

        c=sizeof(struct sockaddr_in);
	int i=0;
       while(client_sock=accept(socket_desc,(struct sockaddr*)&client,(socklen_t*)&c))
       {
        puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        puts("Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    return 0;
}
/*
  This will handle connection for each client
  */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
	int k=i,z,m=0;
     socket1[i++] = *(int*)socket_desc;
	
    int n,j;

        
	//printf("sock=%d\n",sock);
      while((n=recv(socket1[k],client_message,2000,0))>0)
      {
	printf("Received string from Client %d= %s", socket1[k], client_message);
	j=i;
	manipulating_string(client_message,socket1[k]);
	n=strlen(sendBuff);
	for(z=j;z>=0;z--){
		if(socket1[z] !=socket1[k])
		{
		send(socket1[z],sendBuff,n,0);
		}
	     }
 	 bzero(sendBuff,SIZE);
	memset(client_message,0,2000);
	}
      close(socket1[k]);

      if(n==0)
      {
        puts("Client Disconnected");
      }
      else
      {
        perror("recv failed");
      }
    return 0;
}
