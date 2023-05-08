program: main.o HackEnrollment.o IsraeliQueue.o
	gcc main.o HackEnrollment.o IsraeliQueue.o  -o HackEnrollment

IsraeliQueue.o: IsraeliQueue.c IsraeliQueue.h
	gcc -c IsraeliQueue.c

main.o: main.c IsraeliQueue.h HackEnrollment.h
	gcc -c main.c

