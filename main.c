#include "engine/engine.h"
#include <jTypes.h>
#include <SDL2/SDL.h>

#include <cairo.h>
#include <math.h>

#include <stdio.h>

#include "actors/frameRateBarActor/frameRateBarActor.h"
#include "actors/collisionDiagramActor/collisionDiagramActor.h"
#include "actors/movingPointActor/movingPointActor.h"

#include "herdingSheepsEngine/herdingSheepsEngine.h"

void herdingSheepsPreLogic(engine * e)
{
}

herdingSheepsEngine * initHerdingSheepsEngine(herdingSheepsEngine * eng)
{
    eng->engine = createEngine(800, 600, eng);

    enginePreLogicCallBackReg(eng->engine, herdingSheepsPreLogic);

    // setup collisionDiagram
    initCollisionDiagram(eng->engine, &eng->collisionDiagram);

    // setup frameRateBar
    initFrameRateBar(eng->engine, &eng->frameRateBar);

    // setup bluePoint
    eng->bluePoint.a.owner = &eng->bluePoint;
    eng->bluePoint.a.renderHandler = NULL;
    eng->bluePoint.a.logicHandler = NULL; // bluePointLogicHandler;
    eng->bluePoint.rStart[0] = 50;
    eng->bluePoint.rStart[1] = 50;
    eng->bluePoint.frameStart = 0;
    eng->bluePoint.targFrame  = 80;
    eng->bluePoint.sTarg[0]  = 150;
    eng->bluePoint.sTarg[1]  = 150;

    return eng;
}

int main()
{
    herdingSheepsEngine eng;
    initHerdingSheepsEngine(&eng);

    engineStart(eng.engine);
}
