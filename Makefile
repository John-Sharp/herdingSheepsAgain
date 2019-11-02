CC = gcc

CFLAGS = -g -Wall -lm -lSDL2 -lSDL2_image -I /home/john/code/cairo/lib/include/cairo -L /home/john/code/cairo/lib/lib -lcairo -Wl,-rpath=/home/john/code/cairo/lib/lib


jTypesT: jTypes/jTypes.h jTypes/jTypes.c
	$(CC) jTypes/jTypes.c -g -Wall -lm -fpic -c

generateInputProcessorListCode: dataStructures/templates/listTemplate.h dataStructures/templates/listTemplate.inc
	dataStructures/generateList.py engine/inputProcessor/listHeaders engine/inputProcessor/listCode mouseCallbackBinding:mouseCallbackBinding

inputProcessor: generateInputProcessorListCode jTypesT engine/inputProcessor/inputProcessor.c engine/inputProcessor/inputProcessor.h
	$(CC) -IjTypes engine/inputProcessor/inputProcessor.c -g -Wall -lm -fpic -c

test : jTypesT test/test.c
	$(CC) -IjTypes jTypes.o test/test.c -g -Wall -lm -o test

frameRateTracker: engine/frameRateTracker/frameRateTracker.h engine/frameRateTracker/frameRateTracker.c
	$(CC) engine/frameRateTracker/frameRateTracker.c $(CFLAGS) -c

engineLists: engine/listHeaders engine/listCode
	dataStructures/generateList.py engine/listHeaders engine/listCode preLogicCallBack:preLogicCallBack preRenderCallBack:preRenderCallBack

engine: jTypesT inputProcessor frameRateTracker engine/engine.h engine/engine.c engineLists
	$(CC) -IjTypes engine/engine.c $(CFLAGS) -c

frameRateBarActor: actors/frameRateBarActor/frameRateBarActor.h actors/frameRateBarActor/frameRateBarActor.c
	$(CC) -IjTypes actors/frameRateBarActor/frameRateBarActor.c $(CFLAGS) -c

collisionDiagramActor: actors/collisionDiagramActor/collisionDiagramActor.h actors/collisionDiagramActor/collisionDiagramActor.c
	$(CC) -IjTypes -IcollDetect actors/collisionDiagramActor/collisionDiagramActor.c $(CFLAGS) -c

collDetectT: collDetect/collDetect.h collDetect/collDetect.c
	$(CC) -IjTypes collDetect/collDetect.c $(CFLAGS) -c

generateHerdingSheepsEngineListCode: dataStructures/templates/listTemplate.h dataStructures/templates/listTemplate.inc
	dataStructures/generateList.py herdingSheepsEngine/listHeaders herdingSheepsEngine/listCode wallActor:wallActor

movingPointActor: actors/movingPointActor/movingPointActor.h actors/movingPointActor/movingPointActor.c
	$(CC) -IjTypes -IcollDetect actors/movingPointActor/movingPointActor.c $(CFLAGS) -c

herdingSheepsEngineT: herdingSheepsEngine/herdingSheepsEngine.h herdingSheepsEngine/herdingSheepsEngine.c generateHerdingSheepsEngineListCode
	$(CC) -IjTypes -IcollDetect herdingSheepsEngine/herdingSheepsEngine.c $(CFLAGS) -c

wallActor: actors/wallActor/wallActor.h actors/wallActor/wallActor.c
	$(CC) -IjTypes -IcollDetect actors/wallActor/wallActor.c $(CFLAGS) -c

all: engine herdingSheepsEngineT frameRateBarActor collisionDiagramActor movingPointActor wallActor collDetectT main.c 
	$(CC) -IjTypes -IcollDetect main.c engine.o herdingSheepsEngine.o inputProcessor.o jTypes.o frameRateTracker.o frameRateBarActor.o collisionDiagramActor.o movingPointActor.o wallActor.o collDetect.o $(CFLAGS) -o herdingSheeps
