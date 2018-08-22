#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_SIZE 2000
pthread_t threads1;
char s[MAX_SIZE];
int sock_desc;


void* Read(void * socket){
	int sockfd;
	int l=0,client_no;
	sockfd = (intptr_t ) socket;
	while(1){
		memset(s,0,MAX_SIZE);
		recv(sockfd, s, MAX_SIZE, 0);
		//l=strlen(s);
		//client_no=(int)s[l-1];
		//s[l-1]='\0';
		printf(" %s\n",s);
	 bzero(s,MAX_SIZE);
	}
}

int main()
{
    
    struct sockaddr_in serv_addr;
    char sbuff[MAX_SIZE],rbuff[MAX_SIZE];

    if((sock_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf("Failed creating socket\n");

    bzero((char *) &serv_addr, sizeof (serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(3000);

    if (connect(sock_desc, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) {
        printf("Failed to connect to server\n");
        return -1;
    }

	printf("Connected successfully - Please enter string\n");

	pthread_attr_t pthread_custom_attr1;
	
	pthread_attr_init(&pthread_custom_attr1);
	pthread_create(&threads1,&pthread_custom_attr1,Read,(void *)(intptr_t) (sock_desc));

        while(fgets(sbuff, MAX_SIZE , stdin)!=NULL)
    {
	//printf("sent string from client=%s",sbuff);
      	send(sock_desc,sbuff,strlen(sbuff),0);

         /* if(recv(sock_desc,rbuff,MAX_SIZE,0)==0)
           printf("Error");
          else
           fputs(rbuff,stdout);*/

      
    }
        close(sock_desc);
    return 0;

}
