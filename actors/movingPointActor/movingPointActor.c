#include "movingPointActor.h" 

#include <SDL2/SDL.h>

#include <cairo.h>

void initMovingPointActor(engine * eng, movingPointActor * a)
{
    a->a.owner = a;
    a->a.renderHandler = NULL;
    a->a.logicHandler = NULL; // bluePointLogicHandler;
    jintVec rStart = {50, 50};
    jintVec sTarg = {0, 0};
    a->vLine = createJintLine(rStart, sTarg, 80);
    a->frameStart = 0;
}

void movingPointActorGetPosition(movingPointActor * a, jint * rx, jint * ry, juint frame)
{
    jintVec r = jintLineGetPosition(&a->vLine, (jint)frame - (jint)a->frameStart);
    *rx = r.x;
    *ry = r.y;
}
