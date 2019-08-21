#ifndef WALL_ACTOR_H
#define WALL_ACTOR_H

#include "../../engine/engine.h"
#include <jTypes.h>

typedef struct wallActor {
    actor a;

    jintVec pos;
    jint length;
} wallActor;

void initWallActor(engine * eng, wallActor * a);
#endif
