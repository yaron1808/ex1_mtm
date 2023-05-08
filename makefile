CC = gcc
OBJS = main.o HackEnrollment.o IsraeliQueue.o
EXEC = program
DEBUG_FLAG = # now empty, assign -g for debug
COMP_FLAG = t -lm -I/home/mtm/public/2223b/ex1 -Itool -Wall -pedantic-errors -Werror -DNDEBUG

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o HackEnrollment

IsraeliQueue.o: IsraeliQueue.c IsraeliQueue.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

main.o: main.c IsraeliQueue.h HackEnrollment.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

clean:
	rm -f $(OBJS) $(EXEC)