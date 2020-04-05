#ifndef POINT_ACTOR_H
#define POINT_ACTOR_H

#include "../../engine/engine.h"
#include <jTypes.h>
#include <collDetect.h>

#include "../objectActor/objectActor.h"

typedef struct pointActor {
    actor a;
    sprite s;
    decal d;

    collActor ca;

    objectActor oa;
} pointActor;

pointActor * createPointActor(engine * eng);
void initPointActor(engine * eng, pointActor * a);
void pointActorDeinit(pointActor * a);
void pointActorGetPosition(pointActor * a, jintVec * r);
void pointActorGetPositionAtFrame(
        pointActor * a, jint frame, jintVec * r);

#endif
