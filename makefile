CC = gcc
OBJS = main.o HackEnrollment.o IsraeliQueue.o
EXEC = program
DEBUG_FLAG = # now empty, assign -g for debug
COMP_FLAG = -std=c99 -lm -I/home/mtm/public/2223b/ex1 -Itool -Wall -pedantic-errors -Werror -DNDEBUG

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(COMP_FLAG) $(OBJS) -o HackEnrollment

IsraeliQueue.o: IsraeliQueue.c IsraeliQueue.h

main.o: main.c IsraeliQueue.h HackEnrollment.h

HackEnrollment.o: HackEnrollment.c HackEnrollment.h IsraeliQueue.h

clean:
	rm -f $(OBJS) $(EXEC)