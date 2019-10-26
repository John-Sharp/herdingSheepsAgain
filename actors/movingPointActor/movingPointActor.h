#ifndef MOVING_POINT_ACTOR_H
#define MOVING_POINT_ACTOR_H

#include "../../engine/engine.h"
#include <jTypes.h>

typedef struct movingPointActor {
    actor a;
    sprite s;
    decal d;
    jintLine vLine; // line describing the actor's velocity
    juint frameStart;
} movingPointActor;

void initMovingPointActor(engine * eng, movingPointActor * a);
void movingPointActorGetPosition(movingPointActor * a, jint * rx, jint * ry, juint frame);

#endif
