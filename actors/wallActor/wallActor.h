#ifndef WALL_ACTOR_H
#define WALL_ACTOR_H

#include "../../engine/engine.h"
#include <jTypes.h>

typedef struct wallActor {
    actor a;

    jintAxPlLine line;
} wallActor;

void initWallActor(engine * eng, wallActor * a);
#endif
