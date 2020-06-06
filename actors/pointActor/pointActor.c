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
    this->ca.shape.point = engineGetMouseLocation(this->a.eng);
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

void pointActorSetVelocity(
        objectActor * oa, const jintVecScaled * v)
{
    pointActor * this = oa->ptr.pa;
    collEngineCollActorSetVelocity(
            ((herdingSheepsEngine *)this->a.eng->owner)->collEngine,
            &this->ca, v);
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
                jintVec lStart;
                jintVec lEnd = engineGetMouseLocation(this->a.eng);

                lStart = this->ca.shape.point;
                jintVecScaled vel;
                vel.v = jintVecSub(lEnd, lStart);
                vel.s = 80;
                pointActorSetVelocity(
                        &this->oa, 
                        &vel);
                // this->ca.frameStart = a->eng->currentFrame;
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
        },
        .categoryNumber = HS_COLLISION_CATEGORY,
        .nextScheduledCollision = NULL
    };
    this->ca = ca;
    collEngineRegisterCollActor(
            ((herdingSheepsEngine *)eng->owner)->collEngine,
            &this->ca);

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
    collEngineDeregisterCollActor(
            ((herdingSheepsEngine *)this->a.eng->owner)->collEngine,
            &this->ca);
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
