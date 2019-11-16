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
                .v = {50,50},
                .scale = 1 
            }
        },
        .vel = {
            .v = {0,0},
            .scale = 80
        }
    };
    a->ca = ca;
    a->frameStart = 0;
}

void movingPointActorGetPosition(movingPointActor * a, jint * rx, jint * ry, juint frame)
{
    jintLine l = {
        .rStart = a->ca.shape.point,
        .sTarg = a->ca.vel,
        .tScale = a->ca.vel.scale
    };
    jintVec r = jintLineGetPosition(&l, (jint)frame - (jint)a->frameStart);
    *rx = r.v[0];
    *ry = r.v[1];
}
