CC = gcc 
OBJS1 = event.o date.o event_manager.o member.o event_manager_tests.o
OBJS2 = priority_queue.o pq_tests.o node.o
EXEC1 = event_manager 
EXEC2 = priority_queue
DEBUG = -pedantic-errors -DNDEBUG -g 
CFLAGS = -std=c99 -Wall  -Werror
LPQ =  -L. -lpriority_queue

$(EXEC1) : $(OBJS1) 
	$(CC) $(DEBUG) $(CFLAGS) $(OBJS1) -o $@ $(LPQ)

$(EXEC2) : $(OBJS2)
	$(CC) $(DEBUG) $(CFLAGS) $(OBJS2) -o $@

pq_tests.o: tests/pq_tests.c priority_queue.h tests/test_utilities.h
	$(CC) -c $(DEBUG) $(CFLAGS) $/tests/pq_tests.c

event_manager_tests.o: tests/event_manager_tests.c event_manager.h tests/test_utilities.h
	$(CC) -c $(DEBUG) $(CFLAGS) $/tests/event_manager_tests.c

priority_queue.o: priority_queue.c priority_queue.h node.h
	$(CC) -c $(DEBUG) $(CFLAGS) $*.c

node.o : node.c node.h
	$(CC) -c $(DEBUG) $(CFLAGS) $*.c
	
date.o: date.c date.h
	$(CC) -c $(DEBUG) $(CFLAGS) $*.c
  
event.o: event.c event.h member.h date.h priority_queue.h
	$(CC) -c $(DEBUG) $(CFLAGS) $*.c

event_manager.o: event_manager.c event_manager.h date.h priority_queue.h member.h event.h
	$(CC) -c $(DEBUG) $(CFLAGS) $*.c

member.o: member.c member.h
	$(CC) -c $(DEBUG) $(CFLAGS) $*.c
  
clean:
	rm -f $(OBJS1) $(OBJS2) $(EXEC1) $(EXEC2)