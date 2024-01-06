#---------------------------------------------------
# Name: Filippo Ciandy, Rowan Tilroe
# ID:1641346,1629172
# CMPUT 275, Winter 2020
# Assingment 2 Part 1: Directions
#---------------------------------------------------
CC = g++
CFLAGS = -c -Wall
LFLAGS = -static

server: serialport.o server.o digraph.o dijkstra.o 
	$(CC) serialport.o server.o digraph.o dijkstra.o  -o server

server.o: server.cpp heap.h dijkstra.h wdigraph.h serialport.h
	$(CC) server.cpp -o server.o $(CFLAGS)

digraph.o: digraph.cpp digraph.h
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

dijkstra.o: dijkstra.cpp wdigraph.h heap.h
	$(CC) dijkstra.cpp -o dijkstra.o $(CFLAGS)

serialport.o: serialport.cpp serialport.h
	$(CC) serialport.cpp -o serialport.o $(CFLAGS)

clean:
	@rm -f server server.o digraph.o dijkstra.o serialport.o