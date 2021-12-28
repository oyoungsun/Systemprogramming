all : student.c teacher.c bc2dc.o
	gcc -o teacher teacher.c bc2dc.o -lpthread
	gcc -o student student.c bc2dc.o -lpthread

bc2dc.o : bc2dc.c
	gcc -c bc2dc.c -o bc2dc.o
