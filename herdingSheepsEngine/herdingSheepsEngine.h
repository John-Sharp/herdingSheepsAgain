#ifndef HERDING_SHEEPS_ENGINE_H
#define HERDING_SHEEPS_ENGINE_H

#include "../engine/engine.h"
#include <jTypes.h>
#include <collDetect.h>

#include "../actors/collisionDiagramActor/collisionDiagramActor.h"
#include "../actors/frameRateBarActor/frameRateBarActor.h"
#include "../actors/movingPointActor/movingPointActor.h"
#include "../actors/wallActor/wallActor.h"

#include "listHeaders/wallActorList.h"

typedef struct herdingSheepsEngine {
    engine * engine;

    collisionDiagramActor collisionDiagram;
    frameRateBarActor frameRateBar;

    movingPointActor bluePoint;

    wallActorList * walls;

    jintRect mainWindow;
} herdingSheepsEngine;

herdingSheepsEngine * initHerdingSheepsEngine(herdingSheepsEngine * eng);

#endif
