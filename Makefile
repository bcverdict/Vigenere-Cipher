PASC: main.o Executive.o
	g++ -std=c++11 -g -Wall main.o Executive.o -o PASC

main.o: main.cpp
	g++ -std=c++11 -g -Wall -c main.cpp

Executive.o: Executive.cpp Executive.h
	g++ -std=c++11 -g -Wall -c Executive.cpp	

mem:
	valgrind --leak-check=full ./PASC

clean:
	rm *.o PASC

