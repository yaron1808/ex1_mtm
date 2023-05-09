CC = gcc
OBJECTS = main.o HackEnrollment.o IsraeliQueue.o
EXEC = HackEnrollment
DEBUG_FLAG = # now empty, assign -g for debug
ISRAELI_QUEUE_DIR = /new_home/course/mtm/public/2223b/ex1
TOOL_DIR = tool/
COMP_FLAG = -std=c99 -lm -I$(ISRAELI_QUEUE_DIR) -I$(TOOL_DIR) -Wall -pedantic-errors -Werror -DNDEBUG
COMP_FROM_TOOL_DIR = $(CC) $(DEBUG_FLAG) $(COMP_FLAG) -c $(TOOL_DIR)/$*.c -o $@

program : $(OBJECTS)
	$(CC) $(DEBUG_FLAG) $(COMP_FLAG) $(OBJECTS) -o $(EXEC)

IsraeliQueue.o: IsraeliQueue.c IsraeliQueue.h

main.o: $(TOOL_DIR)main.c $(TOOL_DIR)HackEnrollment.h
	$(COMP_FROM_TOOL_DIR)

HackEnrollment.o: $(TOOL_DIR)HackEnrollment.c $(TOOL_DIR)HackEnrollment.h $(ISRAELI_QUEUE_DIR)IsraeliQueue.h
	$(COMP_FROM_TOOL_DIR)
clean:
	rm -f $(OBJECTS) $(EXEC)