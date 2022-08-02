# -----------------------------------------------
# Name:  Aryan Rusia
# SID:   1669003
# CCID:  rusia
# CMPUT 275 Winter 2022
# Assignment 1 - Trivial Navigation System (Part 1)
# ----------------------------------------------

CC = g++
PROGRAM = server
OBJS = digraph.o dijkstra.o server.o
FLAGS = -c -Wall -std=c++11

server: digraph.o dijkstra.o server.o 
	$(CC) $(OBJS) -o server 

digraph.o: digraph.cpp digraph.h wdigraph.h
	$(CC) digraph.cpp -o digraph.o $(FLAGS)

dijkstra.o: dijkstra.cpp digraph.h wdigraph.h 
	$(CC) dijkstra.cpp -o dijkstra.o $(FLAGS)

server.o: server.cpp digraph.h wdigraph.h
	$(CC) server.cpp -o server.o $(FLAGS)

# clean
clean:
	@rm $(PROGRAM) $(OBJS)
