#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "bc2dc.h"

#define oops(m,x) {perror(m); exit(x);}


void fatal(char mess[])
{
	fprintf(stderr, "Error: %s\n", mess);
	exit(1);
}


void error_handling(char* message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

void be_dc(int in[2], int out[2])
{
	if ( dup2(in[0], 0) == -1 )
		oops("dc: cannot redirect stdin", 3);
	close(in[0]);
	close(in[1]);

	if ( dup2(out[1], 1) == -1 )
		oops("dc: cannot redirect stdout", 4);
	close(out[1]);
	close(out[0]);

	execlp("dc", "dc", "-", NULL );
	oops("Cannot run dc", 5);
}

char * be_bc(int todc[0], int fromdc[2], char *message, char* msg)
{
	char *fgets();
	FILE *fpout, *fpin, *fdopen();
	close(todc[0]);
	close(fromdc[1]);
	fpout = fdopen( todc[1], "w" );
	fpin = fdopen( fromdc[0], "r" );
	if ( fpout == NULL || fpin == NULL )
		fatal("Error converting pipes to streams");
	//	while ( printf("tinybc: "), message != NULL){
	
	char operation = find_operation(message); 
	if ( fprintf( fpout, "%d\n%d\n%c\np\n", num1, num2, operation ) == EOF )
		fatal("Error writing");
	fflush(fpout);
	if ( fgets( msg, BUFSIZ, fpin ) == NULL )
		printf("read from dc error");	
//	}i
//	printf("real answer from bc : %s\n", msg);
	fclose(fpout);
	fclose(fpin);

	return msg;
}

char find_operation(char* message){
	int i;
	char *delim = "-+*/^ ";
	char *number="0123456789 ";
	char temp[BUFSIZ];
	
	strcpy(temp, message);
	char *n1 = strtok(temp, delim); //num1
//	char *tmp = strtok(NULL, ""); //oper
	int k = strlen(n1);
	num1 = atoi(n1);
//	printf("num1 is %d \n",num1);
//	strcpy(operation, tmp);
//	printf("operation is, %s \n", tmp);
	char *n2 = strtok(NULL, ""); //num2
	num2 = atoi(n2);
//	printf("num2 is %d\n", num2);

	return message[k];
}

void bc2dc(char *message, char * answer){
	int pid, todc[2], fromdc[2];
	if(pipe(todc) == -1 || pipe(fromdc)==-1)
		oops("pipe failed", 1);
	if((pid = fork()) == -1)
		oops("cannot fork", 2);
	if(pid == 0)
		be_dc(todc, fromdc);
	else{
		be_bc(todc, fromdc, message, answer);
		wait(NULL);
	}
}

