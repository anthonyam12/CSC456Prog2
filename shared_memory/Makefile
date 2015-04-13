CC = g++

LIBS =

FLAGS = -Wall

EXECS = dash

all: $(EXECS)

prog1.o: prog1.cpp prog1.h
	${CC} ${CFLAGS} -c prog1.cpp

prog2.o: prog2.cpp prog2.h
	${CC} ${CFLAGS} -c prog2.cpp

dash.o: dash.cpp
	${CC} ${CFLAGS} -c dash.cpp

dash: dash.o prog1.o prog2.o
	$(CC) $(FLAGS) dash.o prog1.o prog2.o -o dash 

clean:
	$(RM) $(EXECS) *.o

