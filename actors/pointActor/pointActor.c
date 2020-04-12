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

static void pointActorSetPositionToMouseLocation(
        pointActor * this)
{
    int mouse_x, mouse_y;

    SDL_GetMouseState(&mouse_x, &mouse_y);
    this->ca.shape.point.v[0] = mouse_x;
    this->ca.shape.point.v[1] = this->a.eng->h - mouse_y;
}

static bool pointActorIsFocussedActor(
        pointActor * this)
{
    herdingSheepsEngine * hsEng = this->a.eng->owner;
    objectActor * focussedActor = hsEng->objectActorList->val;

    if (focussedActor->type != OBJECT_ACTOR_TYPE_POINT)
        return false;

    if (focussedActor->ptr.pa == this)
        return true;

    return false;
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
        case HS_GAME_STATE_CHOOSE_OBJECT:
        case HS_GAME_STATE_OBJECT_CHOOSE_DIMENSION:
        {
            this->ca.frameStart = a->eng->currentFrame;
            break;
        }
        case HS_GAME_STATE_OBJECT_CHOOSE_VELOCITY:
        {
            this->ca.frameStart = a->eng->currentFrame;
            if (pointActorIsFocussedActor(this))
            {
                jintVec lStart, lEnd;
                SDL_GetMouseState(&lEnd.v[0], &lEnd.v[1]);
                lEnd.v[1] = a->eng->h - lEnd.v[1];

                lStart = this->ca.shape.point;
                this->ca.vel.v = jintVecSub(lEnd, lStart);
                this->ca.vel.s = 80;
                this->ca.frameStart = a->eng->currentFrame;
            }
            break;
        }
        case HS_GAME_STATE_OBJECT_BEING_POSITIONED:
        {
            this->ca.frameStart = a->eng->currentFrame;
            if (pointActorIsFocussedActor(this))
            {
                pointActorSetPositionToMouseLocation(this);
            }
            break;
        }
        default:
            break;
    }
}

void pointActorSetVelocity(
        objectActor * oa, const jintVecScaled * v)
{
    pointActor * this = oa->ptr.pa;
    this->ca.vel = *v;
}

collActor * pointActorGetCollActor(
        const objectActor * oa)
{
    pointActor * this = oa->ptr.pa;

    return &this->ca;
}

static void pointActorResetAppearence(objectActor * oa){}
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
    this->oa.objectActorSetVelocity = pointActorSetVelocity;
    this->oa.objectActorGetCollActor = pointActorGetCollActor;
    this->oa.objectActorResetAppearence = pointActorResetAppearence;

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
    *r = jintLineGetPosition(&l, frame);
}
