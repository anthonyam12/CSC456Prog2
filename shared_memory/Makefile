CC = g++

LIBS =

FLAGS = -Wall -pthread -g

EXECS = dash

all: $(EXECS)

prog1.o: prog1.cpp prog1.h
	${CC} ${CFLAGS} -c prog1.cpp

prog2.o: prog2.cpp prog2.h
	${CC} ${CFLAGS} -c prog2.cpp

prog3.o: prog3.cpp prog3.h
	${CC} ${CFLAGS} -c prog3.cpp

dash.o: dash.cpp
	${CC} ${CFLAGS} -c dash.cpp

dash: dash.o prog1.o prog2.o prog3.o
	$(CC) $(FLAGS) dash.o prog1.o prog2.o prog3.o -o dash 

clean:
	$(RM) $(EXECS) *.o

