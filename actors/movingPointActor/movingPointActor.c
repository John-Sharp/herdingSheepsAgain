#include "movingPointActor.h" 

#include <SDL2/SDL.h>

#include <cairo.h>

void initMovingPointActor(engine * eng, movingPointActor * a)
{
    a->a.owner = a;
    a->a.renderHandler = NULL;
    a->a.logicHandler = NULL; // bluePointLogicHandler;
    a->rStart[0] = 50;
    a->rStart[1] = 50;
    a->frameStart = 0;
    a->targFrame  = 80;
    a->sTarg[0]  = 150;
    a->sTarg[1]  = 150;
}

void movingPointActorGetPosition(movingPointActor * a, jint * rx, jint * ry, juint frame)
{
    *rx = a->rStart[0] + a->sTarg[0] * (frame - a->frameStart) / a->targFrame;
    *ry = a->rStart[1] + a->sTarg[1] * (frame - a->frameStart) / a->targFrame;
}
