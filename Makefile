CC = gcc

CFLAGS = -g -Wall -lm -lSDL2 -lSDL2_image -I /home/john/code/cairo/lib/include/cairo -L /home/john/code/cairo/lib/lib -lcairo -Wl,-rpath=/home/john/code/cairo/lib/lib


jTypes: jTypes/jTypes.h jTypes/jTypes.c
	$(CC) jTypes/jTypes.c -g -Wall -lm -fpic -c

generateInputProcessorListCode: dataStructures/templates/listTemplate.h dataStructures/templates/listTemplate.inc
	dataStructures/generateList.py engine/inputProcessor/listHeaders engine/inputProcessor/listCode mouseCallbackBinding:mouseCallbackBinding

inputProcessor: generateInputProcessorListCode jTypes engine/inputProcessor/inputProcessor.c engine/inputProcessor/inputProcessor.h
	$(CC) -IjTypes engine/inputProcessor/inputProcessor.c -g -Wall -lm -fpic -c

test : jTypes test/test.c
	$(CC) -IjTypes jTypes.o test/test.c -g -Wall -lm -o test

frameRateTracker: engine/frameRateTracker/frameRateTracker.h engine/frameRateTracker/frameRateTracker.c
	$(CC) engine/frameRateTracker/frameRateTracker.c $(CFLAGS) -c

engine: jTypes inputProcessor frameRateTracker engine/engine.h engine/engine.c 
	$(CC) -IjTypes engine/engine.c $(CFLAGS) -c

frameRateBarActor: actors/frameRateBarActor/frameRateBarActor.h actors/frameRateBarActor/frameRateBarActor.c
	$(CC) -IjTypes actors/frameRateBarActor/frameRateBarActor.c $(CFLAGS) -c

collisionDiagramActor: actors/collisionDiagramActor/collisionDiagramActor.h actors/collisionDiagramActor/collisionDiagramActor.c
	$(CC) -IjTypes actors/collisionDiagramActor/collisionDiagramActor.c $(CFLAGS) -c

movingPointActor: actors/movingPointActor/movingPointActor.h actors/movingPointActor/movingPointActor.c
	$(CC) -IjTypes actors/movingPointActor/movingPointActor.c $(CFLAGS) -c

herdingSheepsEngine: herdingSheepsEngine/herdingSheepsEngine.h herdingSheepsEngine/herdingSheepsEngine.c
	$(CC) -IjTypes herdingSheepsEngine/herdingSheepsEngine.c $(CFLAGS) -c

all: engine herdingSheepsEngine frameRateBarActor collisionDiagramActor movingPointActor main.c 
	$(CC) -IjTypes main.c engine.o herdingSheepsEngine.o inputProcessor.o jTypes.o frameRateTracker.o frameRateBarActor.o collisionDiagramActor.o movingPointActor.o $(CFLAGS) -o herdingSheeps
