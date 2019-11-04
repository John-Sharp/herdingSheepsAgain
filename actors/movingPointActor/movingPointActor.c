#include "movingPointActor.h" 

#include <SDL2/SDL.h>

#include <cairo.h>

void initMovingPointActor(engine * eng, movingPointActor * a)
{
    a->a.owner = a;
    a->a.renderHandler = NULL;
    a->a.logicHandler = NULL; // bluePointLogicHandler;

    collActor ca = { 
        .type = COLL_ACTOR_TYPE_POINT,
        .shape = {
            .point = {
                .rStart = {{50,50}},
                .sTarg = {{0, 0}},
                .tScale = 80
            }
        }
    };
    a->ca = ca;
    a->pos = &a->ca.shape.point;
    a->frameStart = 0;
}

void movingPointActorGetPosition(movingPointActor * a, jint * rx, jint * ry, juint frame)
{
    jintVec r = jintLineGetPosition(a->pos, (jint)frame - (jint)a->frameStart);
    *rx = r.v[0];
    *ry = r.v[1];
}
