CC = gcc 
CPP = g++

CFLAGS = -c 

simulation: $(OBJS)
	$(CPP) -std=c++11 -lm simulation_code/main.cpp -o simulation
