CC = gcc

CFLAGS = -g -Wall -lm -lSDL2 -lSDL2_image

jTypes: jTypes/jTypes.h jTypes/jTypes.c
	$(CC) jTypes/jTypes.c -g -Wall -lm -fpic -c

inputProcessor: jTypes engine/inputProcessor/inputProcessor.c engine/inputProcessor/inputProcessor.h
	$(CC) -IjTypes engine/inputProcessor/inputProcessor.c -g -Wall -lm -fpic -c

test : jTypes test/test.c
	$(CC) -IjTypes jTypes.o test/test.c -g -Wall -lm -o test

engine: jTypes inputProcessor engine/engine.h engine/engine.c  
	$(CC) -IjTypes engine/engine.c $(CFLAGS) -c

all: engine main.c 
	$(CC) -IjTypes main.c engine.o inputProcessor.o jTypes.o $(CFLAGS) -o herdingSheeps
