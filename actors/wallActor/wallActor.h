#ifndef WALL_ACTOR_H
#define WALL_ACTOR_H

#include "../../engine/engine.h"
#include <jTypes.h>
#include <collDetect.h>

typedef struct herdingSheepsEngine herdingSheepsEngine;

typedef struct wallActor {
    actor a;

    collActor ca;
    const jintAxPlLine * line;
} wallActor;

void initWallActors(herdingSheepsEngine * eng);
#endif
