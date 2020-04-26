all:httpServer

CFLAGS = -O2 -pthread -lpthread
SRC = main.cpp 

httpServer: $(SRC)
	g++ $(CFLAGS) $(SRC) -o httpserver
	
clean:
	-rm -f httpserver