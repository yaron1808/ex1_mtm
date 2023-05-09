CC = gcc
OBJECTS = main.o HackEnrollment.o IsraeliQueue.o
EXEC = HackEnrollment
DEBUG_FLAG = # now empty, assign -g for debug
QUEUE_DIR = /new_home/courses/mtm/public/2223b/ex1
TOOL_DIR = tool
CFLAGS = -std=c99 -lm -I$(QUEUE_DIR) -I$(TOOL_DIR) -Wall -pedantic-errors -Werror -DNDEBUG
TOOL_DIR_COMP = $(CC) $(DEBUG_FLAG) $(CFLAGS) -c $(TOOL_DIR)/$*.c -o $@

program: $(OBJECTS)
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $(OBJECTS) -o $(EXEC)

main.o: $(TOOL_DIR)/main.c $(TOOL_DIR)/HackEnrollment.h
	$(TOOL_DIR_COMP)

HackEnrollment.o: $(TOOL_DIR)/HackEnrollment.c $(TOOL_DIR)/HackEnrollment.h $(QUEUE_DIR)/IsraeliQueue.h
	$(TOOL_DIR_COMP)

IsraeliQueue.o: IsraeliQueue.c $(QUEUE_DIR)/IsraeliQueue.h

clean:
	rm -f $(OBJECTS) $(EXEC)
