enigma: main.o enigma.o rotor.o plugboard.o reflector.o
	g++ -g main.o enigma.o rotor.o plugboard.o reflector.o -o enigma

main.o: main.cpp components.h
	g++ -Wall -g -c main.cpp -o main.o

reflector.o: Reflector.cpp components.h
	g++ -Wall -g -c Reflector.cpp -o reflector.o

rotor.o: Rotor.cpp components.h
	g++ -Wall -g -c Rotor.cpp -o rotor.o

plugboard.o: Plugboard.cpp components.h
	g++ -Wall -g -c Plugboard.cpp -o plugboard.o

enigma.o: Enigma.cpp components.h
	g++ -Wall -g -c Enigma.cpp -o enigma.o
clean:
	rm -f *.o execute
