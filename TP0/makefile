cc = g++

all: discretefourier

discretefourier: main.o complex.o errors.o
	$(cc) main.cpp complex.cpp errors.cpp -o discretefourier
main.o: main.cpp main.h config.h templatearray.h complex.h errors.h
	$(cc) -c main.cpp
complex.o: complex.cpp complex.h config.h templatearray.h errors.h
	$(cc) -c complex.cpp
errors.o: errors.cpp errors.h config.h
	$(cc) -c errors.cpp
