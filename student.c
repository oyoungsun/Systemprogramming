#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include "bc2dc.h"
clock_t start;
clock_t end;

typedef struct sockdata{
	char* message;
	int* serv_sock;
}sockdata;
char message[30];

void hurryup(int i)
{
	fprintf(stderr, "\nhurry up!\n");
}

void* sendAnswer(void* serv_sock){
	int* cs=(int*)serv_sock;
//timer
	signal(SIGALRM, hurryup);
	alarm(5);
	while(1){
		start = clock();
		printf("\nstudent -> : ");
		scanf("%s",message);
		alarm(5);
		write(*cs,message,sizeof(message));
		if(strcmp("quit", message)==0){
				printf("server disconnect\n");
				break;
		}
	}
}

void* readProblem(void* rcvDt){
	char answer[30];
	sockdata* data=(sockdata*)rcvDt;
	while(1){
	int read_cnt=read(*(data->serv_sock), data->message, sizeof(char)*30);
	if(read_cnt!=-1){
		if(strcmp("quit", message)==0){
				printf("server disconnect\n");
				break;
		}
		printf("\n <- teacher : %s\n", data->message);
	}	
	}
	
}

int main(int argc, char *argv[]){
	
	int serv_sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len;
	
	if(argc!=3){
		printf("Usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}
	//socket
	serv_sock=socket(PF_INET,SOCK_STREAM,0);
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	
	connect(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	//thread
	pthread_t p_thread[2];
	int t;
	int status;
	sockdata rcvDt;
	rcvDt.message=message;
	rcvDt.serv_sock=&serv_sock;
	for(t=0;t<2;t++){
		if(t==0) pthread_create(&p_thread[t],NULL,sendAnswer,(void*)&serv_sock);
		else if(t==1)pthread_create(&p_thread[t],NULL,readProblem,(void*)&rcvDt);
	}
	pthread_join(p_thread[0],(void **)&status);
	pthread_join(p_thread[1],(void **)&status);

	close(serv_sock);
	return 0;
} 
