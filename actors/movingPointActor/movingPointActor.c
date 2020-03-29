#include "movingPointActor.h" 

#include <SDL2/SDL.h>

#include <cairo.h>

#include "../../herdingSheepsEngine/herdingSheepsEngine.h"

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
    herdingSheepsEngine * hsEng = a->eng->owner;
    
    HS_GAME_STATE currentState;
    SBStateMachineGetCurrentState(
        hsEng->mainStateMachine, &currentState);
    switch (currentState)
    {
        case HS_GAME_STATE_MAIN_OBJECT_CHOOSE_VELOCITY:
        {
            // TODO
            break;
        }
        default:
        {
            int mouse_x, mouse_y;

            SDL_GetMouseState(&mouse_x, &mouse_y);
            m->ca.shape.point.v[0] = mouse_x;
            m->ca.shape.point.v[1] = a->eng->h - mouse_y;
            break;
        }
    }
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

void movingPointActorDeinit(movingPointActor * a)
{
    actorEngineDereg(&a->a);
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
