#ifndef HERDING_SHEEPS_ENGINE_H
#define HERDING_SHEEPS_ENGINE_H

#include "../engine/engine.h"
#include <jTypes.h>

#include "../actors/collisionDiagramActor/collisionDiagramActor.h"
#include "../actors/frameRateBarActor/frameRateBarActor.h"
#include "../actors/movingPointActor/movingPointActor.h"

typedef struct herdingSheepsEngine {
    engine * engine;

    collisionDiagramActor collisionDiagram;
    frameRateBarActor frameRateBar;

    movingPointActor bluePoint;

    jintRect mainWindow;
} herdingSheepsEngine;


#endif
