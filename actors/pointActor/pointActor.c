#include "pointActor.h" 

#include <SDL2/SDL.h>

#include <cairo.h>

#include "../../herdingSheepsEngine/herdingSheepsEngine.h"

pointActor * createPointActor(engine * eng)
{
    pointActor * ret;

    ret = malloc(sizeof(*ret));

    if (!ret)
        return ret;

    initPointActor(eng, ret);

    return ret;
}

void pointActorLogicHandler(actor * a)
{
    pointActor * this = a->owner;
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

            lStart = this->ca.shape.point;
            this->ca.vel.v = jintVecSub(lEnd, lStart);
            this->ca.vel.s = 80;
            this->ca.frameStart = a->eng->currentFrame;
            break;
        }
        case HS_GAME_STATE_MAIN_OBJECT_POINT:
        {
            int mouse_x, mouse_y;

            SDL_GetMouseState(&mouse_x, &mouse_y);
            this->ca.shape.point.v[0] = mouse_x;
            this->ca.shape.point.v[1] = a->eng->h - mouse_y;
            break;
        }
        case HS_GAME_STATE_CHOOSE_OTHER_OBJECT:
        case HS_GAME_STATE_OTHER_OBJECT_BEING_POSITIONED:
        {
            this->ca.frameStart = a->eng->currentFrame;
            break;
        }
        default:
            break;
    }
}

void initPointActor(engine * eng, pointActor * this)
{
    this->a.owner = this;
    this->a.renderHandler = NULL;
    this->a.logicHandler = pointActorLogicHandler;

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
    this->ca = ca;

    this->oa.type = OBJECT_ACTOR_TYPE_POINT;
    this->oa.ptr.pa = this;

    engineActorReg(eng, &this->a);
}

void pointActorDeinit(pointActor * this)
{
    actorEngineDereg(&this->a);
}

void pointActorGetPosition(pointActor * this, jintVec * r)
{
    jintLine l = {
        .rStart = this->ca.shape.point,
        .sTarg = this->ca.vel
    };
    *r = jintLineGetPosition(&l, (jint)this->a.eng->currentFrame - this->ca.frameStart);
}

void pointActorGetPositionAtFrame(pointActor * this, jint frame, jintVec * r)
{
    jintLine l = {
        .rStart = this->ca.shape.point,
        .sTarg = this->ca.vel
    };
    *r = jintLineGetPosition(&l, frame - this->ca.frameStart);
}
