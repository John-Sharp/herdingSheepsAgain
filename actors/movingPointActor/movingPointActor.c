#include "movingPointActor.h" 

#include <SDL2/SDL.h>

#include <cairo.h>

movingPointActor * createMovingPointActor(engine * eng)
{
    movingPointActor * ret;

    ret = malloc(sizeof(*ret));

    if (!ret)
        return ret;

    initMovingPointActor(eng, ret);

    return ret;
}

void movingPointActorLogicHandler(actor * a)
{
    movingPointActor * m = a->owner;

    int mouse_x, mouse_y;

    SDL_GetMouseState(&mouse_x, &mouse_y);
    m->ca.shape.point.v[0] = mouse_x;
    m->ca.shape.point.v[1] = a->eng->h - mouse_y;
}

void initMovingPointActor(engine * eng, movingPointActor * a)
{
    a->a.owner = a;
    a->a.renderHandler = NULL;
    a->a.logicHandler = movingPointActorLogicHandler;

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

    engineActorReg(eng, &a->a);
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
