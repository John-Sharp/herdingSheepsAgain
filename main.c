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
    initMovingPointActor(eng->engine, &eng->bluePoint);

    return eng;
}

int main()
{
    herdingSheepsEngine eng;
    initHerdingSheepsEngine(&eng);

    engineStart(eng.engine);
}
