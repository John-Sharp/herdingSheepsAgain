#ifndef MOVING_POINT_ACTOR_H
#define MOVING_POINT_ACTOR_H

#include "../../engine/engine.h"
#include <jTypes.h>

typedef struct movingPointActor {
    actor a;
    sprite s;
    decal d;
    jintVec rStart;
    juint frameStart;
    jint targFrame;
    jintVec sTarg;  // actor will be at point sTarg + r at frame targFrame
                    // (this is how velocity is defined,
                    // v = (sTarg[0] * fps /targFrame[0], sFarg[1] * fps /targFrame[1]))

} movingPointActor;

void movingPointActorGetPosition(movingPointActor * a, jint * rx, jint * ry, juint frame);

#endif
