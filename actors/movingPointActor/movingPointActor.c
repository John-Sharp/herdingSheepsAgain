#include "movingPointActor.h" 

#include <SDL2/SDL.h>

#include <cairo.h>

void movingPointActorGetPosition(movingPointActor * a, jint * rx, jint * ry, juint frame)
{
    *rx = a->rStart[0] + a->sTarg[0] * (frame - a->frameStart) / a->targFrame;
    *ry = a->rStart[1] + a->sTarg[1] * (frame - a->frameStart) / a->targFrame;
}
