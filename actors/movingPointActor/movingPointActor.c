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
            jintVec lStart, lEnd;
            SDL_GetMouseState(&lEnd.v[0], &lEnd.v[1]);
            lEnd.v[1] = a->eng->h - lEnd.v[1];

            lStart = m->ca.shape.point;
            m->ca.vel.v = jintVecSub(lEnd, lStart);
            m->ca.vel.s = 80;
            m->ca.frameStart = a->eng->currentFrame;
            break;
        }
        case HS_GAME_STATE_MAIN_OBJECT_POINT:
        {
            int mouse_x, mouse_y;

            SDL_GetMouseState(&mouse_x, &mouse_y);
            m->ca.shape.point.v[0] = mouse_x;
            m->ca.shape.point.v[1] = a->eng->h - mouse_y;
            break;
        }
        case HS_GAME_STATE_CHOOSE_OTHER_OBJECT:
        {
            m->ca.frameStart = a->eng->currentFrame;
            break;
        }
        default:
            break;
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
            .point = {{50,50}}
        },
        .vel = {
            .v = {{0,0}},
            .s = 80
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

void movingPointActorGetPosition(movingPointActor * a, jintVec * r)
{
    jintLine l = {
        .rStart = a->ca.shape.point,
        .sTarg = a->ca.vel
    };
    *r = jintLineGetPosition(&l, (jint)a->a.eng->currentFrame - a->ca.frameStart);
}
