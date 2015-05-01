CC = gcc 
CPP = g++

CFLAGS = -c 

simulation: $(OBJS)
	$(CPP) -std=c++11 -lm -g simulation_code/main.cpp simulation_code/memory_management.cpp simulation_code/process.cpp -o simulation

clean:
	rm -f simulation_code/*.o *~ core
	rm -f simulation
