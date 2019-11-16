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

void initMovingPointActor(engine * eng, movingPointActor * a);
void movingPointActorGetPosition(movingPointActor * a, jint * rx, jint * ry, juint frame);

#endif
