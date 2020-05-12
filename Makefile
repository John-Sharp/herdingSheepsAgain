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

textBoxActor: actors/textBoxActor/textBoxActor.h actors/textBoxActor/textBoxActor.c
	$(CC) -IjTypes actors/textBoxActor/textBoxActor.c $(CFLAGS) -c

frameRateBar: actors/textBoxActor/frameRateBar/frameRateBar.h actors/textBoxActor/frameRateBar/frameRateBar.c
	$(CC) -IjTypes actors/textBoxActor/frameRateBar/frameRateBar.c $(CFLAGS) -c

infoBox: actors/textBoxActor/infoBox/infoBox.h actors/textBoxActor/infoBox/infoBox.c
	$(CC) -IjTypes actors/textBoxActor/infoBox/infoBox.c $(CFLAGS) -c

collisionDiagramActor: actors/collisionDiagramActor/collisionDiagramActor.h actors/collisionDiagramActor/collisionDiagramActor.c
	$(CC) -IjTypes -IcollDetect -IstudiousBroccoli actors/collisionDiagramActor/collisionDiagramActor.c $(CFLAGS) -c

generateCollDetectListCode: dataStructures/templates/listTemplate.h dataStructures/templates/listTemplate.inc
	mkdir -p collDetect/listHeaders collDetect/listCode
	dataStructures/generateList.py collDetect/listHeaders collDetect/listCode collGroup:collGroup

collDetectT: generateCollDetectListCode collDetect/collDetect.h\
	collDetect/collDetect.c\
	collDetect/collisionFrameCalculate.h\
	collDetect/collisionFrameCalculate.c
	$(CC) -IjTypes collDetect/collDetect.c collDetect/collisionFrameCalculate.c $(CFLAGS) -c

generateHerdingSheepsEngineListCode: dataStructures/templates/listTemplate.h dataStructures/templates/listTemplate.inc
	dataStructures/generateList.py herdingSheepsEngine/listHeaders herdingSheepsEngine/listCode objectActor:objectActor jint:jint

pointActor: actors/pointActor/pointActor.h actors/pointActor/pointActor.c
	$(CC) -IjTypes -IcollDetect -IstudiousBroccoli actors/pointActor/pointActor.c $(CFLAGS) -c

studiousBroccoliT: studiousBroccoli/studiousBroccoli.c studiousBroccoli/studiousBroccoli.h dataStructures/templates/listTemplate.h dataStructures/templates/listTemplate.inc
	./dataStructures/generateList.py studiousBroccoli/listHeaders studiousBroccoli/listCode transitionFnNode:transitionFnNode state:state
	$(CC) -I jTypes studiousBroccoli/studiousBroccoli.c $(CFLAGS) -c

hsStateMachine: herdingSheepsEngine/HSStateMachine/HSStateMachine.h herdingSheepsEngine/HSStateMachine/HSStateMachine.c
	$(CC) -IjTypes -IstudiousBroccoli -IcollDetect herdingSheepsEngine/HSStateMachine/HSStateMachine.c $(CFLAGS) -c

herdingSheepsEngineT: herdingSheepsEngine/herdingSheepsEngine.h herdingSheepsEngine/herdingSheepsEngine.c generateHerdingSheepsEngineListCode
	$(CC) -IjTypes -IcollDetect -IstudiousBroccoli herdingSheepsEngine/herdingSheepsEngine.c $(CFLAGS) -c

lineActor: actors/lineActor/lineActor.h actors/lineActor/lineActor.c
	$(CC) -IjTypes -IcollDetect -IstudiousBroccoli actors/lineActor/lineActor.c $(CFLAGS) -c

rectActor: actors/rectActor/rectActor.h actors/rectActor/rectActor.c
	$(CC) -IjTypes -IcollDetect -IstudiousBroccoli actors/rectActor/rectActor.c $(CFLAGS) -c

objectActor: actors/objectActor/objectActor.h actors/objectActor/objectActor.c
	$(CC) -IjTypes actors/objectActor/objectActor.c $(CFLAGS) -c

all: engine herdingSheepsEngineT textBoxActor frameRateBar infoBox collisionDiagramActor pointActor lineActor rectActor objectActor collDetectT studiousBroccoliT hsStateMachine main.c 
	$(CC) -IjTypes -IcollDetect -IstudiousBroccoli main.c engine.o herdingSheepsEngine.o inputProcessor.o jTypes.o frameRateTracker.o textBoxActor.o frameRateBar.o infoBox.o collisionDiagramActor.o pointActor.o lineActor.o rectActor.o objectActor.o collDetect.o \
		collisionFrameCalculate.o \
		studiousBroccoli.o HSStateMachine.o $(CFLAGS) -o herdingSheeps
