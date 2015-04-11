CC = gcc 
CPP = g++

CFLAGS = -c 

simulation: $(OBJS)
	$(CPP) -lm simulation_code/main.cpp -o simulation
