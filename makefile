CC = gcc 
CPP = g++

CFLAGS = -c 

OBJS = simulation/main.o

simulation: $(OBJS)
	$(CPP) -lm $(OBJS) -o simulation/simulation

