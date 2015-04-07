CC = gcc 
CPP = g++

CFLAGS = -c 

OBJS = simulation/memory_management.o simulation/page_replacement.o simulation/process_scheduler.o simulation/main.o

simulation: $(OBJS)
	$(CPP) -lm $(OBJS) -o simulation/simulation

