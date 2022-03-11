cc = g++

CPPFLAGS  = -Wall -Werror -g

all: fourier

fourier: main.o complex.o errors.o
	$(cc) $(CPPFLAGS) main.cpp complex.cpp errors.cpp -o fourier
main.o: main.cpp main.h config.h templatearray.h complex.h errors.h
	$(cc) $(CPPFLAGS) -c main.cpp
complex.o: complex.cpp complex.h config.h templatearray.h errors.h
	$(cc) $(CPPFLAGS) -c complex.cpp
errors.o: errors.cpp errors.h config.h
	$(cc) $(CPPFLAGS) -c errors.cpp
