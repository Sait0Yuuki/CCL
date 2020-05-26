CFLAGS=-std=c++0x -c

all: main

test: main
	./kvstore2pcsystem

main: parser.o coordinator.o participant.o main.o
	$(CXX) -pthread parser.o coordinator.o participant.o main.o -o kvstore2pcsystem ./lib/librpc.a

#coordinator_test: parser.o coordinator.o
#	$(CXX) -pthread parser.o coordinator.o -o coordinator librpc.a

#g++ -pthread -std=c++0x -c -o coo coordinator.cpp -I include librpc.a
main.o: main.cpp
	$(CXX) $(CFLAGS) main.cpp

parser.o: parser.cpp
	$(CXX) $(CFLAGS) parser.cpp

coordinator.o: coordinator.cpp coordinator.h
	$(CXX) $(CFLAGS) coordinator.cpp -I include

participant.o: participant.cpp participant.h
	$(CXX) $(CFLAGS) participant.h participant.cpp  -I include

clean:
	rm *.o *.h.gch kvstore2pcsystem