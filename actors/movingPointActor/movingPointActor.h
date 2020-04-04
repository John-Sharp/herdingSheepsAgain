#ifndef MOVING_POINT_ACTOR_H
#define MOVING_POINT_ACTOR_H

#include "../../engine/engine.h"
#include <jTypes.h>
#include <collDetect.h>

typedef struct movingPointActor {
    actor a;
    sprite s;
    decal d;

    collActor ca;

    juint frameStart;
} movingPointActor;

movingPointActor * createMovingPointActor(engine * eng);
void initMovingPointActor(engine * eng, movingPointActor * a);
void movingPointActorDeinit(movingPointActor * a);
void movingPointActorGetPosition(movingPointActor * a, jintVec * r);

#endif
