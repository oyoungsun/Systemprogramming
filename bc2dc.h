#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define oops(m,x) {perror(m); exit(x);}

void fatal(char *);
void be_dc(int in[2], int out[2]);
char* be_bc(int todc[2], int fromdc[2], char *message, char *answer);
char find_operation();
void bc2dc(char *message, char *answer);

int num1, num2;
