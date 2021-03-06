#include "rectActor.h" 

#include "../../herdingSheepsEngine/herdingSheepsEngine.h"

rectActor * createRectActor(
        engine * eng,
        const rectActorParams * params)
{
    rectActor * ret;

    ret = malloc(sizeof(*ret));

    if (!ret)
        return ret;

    rectActorInit(ret, eng, params);

    return ret;
}

static void rectActorLogicHandler(actor * a);
static void rectActorSetVelocity(
        objectActor * oa, const jintVecScaled * v);
static collActor * rectActorGetCollActor(
        const objectActor * oa);
static void rectActorResetAppearence(objectActor * oa);

void rectActorInit(
        rectActor * this,
        engine * eng,
        const rectActorParams * params)
{
    this->a.owner = this;
    this->a.renderHandler = NULL;
    this->a.logicHandler = rectActorLogicHandler;

    collActor ca = {
        .type = COLL_ACTOR_TYPE_RECT,
        .shape = {
            .rect = params->rect 
        },
        .vel = {.v = {{0, 0}}, .s = 80},
        .categoryNumber = HS_COLLISION_CATEGORY,
        .nextScheduledCollision = NULL
    };
    this->ca = ca;
    collEngineRegisterCollActor(
            ((herdingSheepsEngine *)eng->owner)->collEngine,
            &this->ca);

    this->originalRect = params->rect;

    this->oa.type = OBJECT_ACTOR_TYPE_RECT;
    this->oa.ptr.ra = this;
    this->oa.objectActorSetVelocity = rectActorSetVelocity;
    this->oa.objectActorGetCollActor = rectActorGetCollActor;
    this->oa.objectActorResetAppearence = rectActorResetAppearence;

    engineActorReg(eng, &this->a);
}

void rectActorDeinit(rectActor * this)
{
    actorEngineDereg(&this->a);
    collEngineDeregisterCollActor(
            ((herdingSheepsEngine *)this->a.eng->owner)->collEngine,
            &this->ca);
}

void rectActorGetRect(
        const rectActor * this,
        jintRect * rect)
{
    *rect = this->ca.shape.rect;
    jintLine l = {
        .rStart = rect->bl,
        .sTarg = this->ca.vel,
    };
    rect->bl = jintLineGetPosition(&l, (jint)this->a.eng->currentFrame - this->ca.frameStart);

    l.rStart = rect->tr;
    rect->tr = jintLineGetPosition(&l, (jint)this->a.eng->currentFrame - this->ca.frameStart);
}

void rectActorGetRectAtFrame(
        const rectActor * this,
        jint frame,
        jintRect * rect)
{
    *rect = this->ca.shape.rect;
    jintLine l = {
        .rStart = rect->bl,
        .sTarg = this->ca.vel,
    };
    rect->bl = jintLineGetPosition(&l, frame);

    l.rStart = rect->tr;
    rect->tr = jintLineGetPosition(&l, frame);
}

static void rectActorSetPositionToMouseLocation(
        rectActor * this);
static void rectActorSetDimensionToMouseLocation(
        rectActor * this);
static void rectActorSetVelocityToMouseLocation(
        rectActor * this);
static bool rectActorIsFocussedActor(
        const rectActor * this);
static void rectActorLogicHandler(actor * a)
{
    rectActor * this = a->owner;
    herdingSheepsEngine * hsEng = a->eng->owner;

    HS_GAME_STATE currentState;
    SBStateMachineGetCurrentState(
        hsEng->mainStateMachine, &currentState);
    switch (currentState)
    {
        case HS_GAME_STATE_OBJECT_BEING_POSITIONED:
        {
            this->ca.frameStart = a->eng->currentFrame;
            if (rectActorIsFocussedActor(this))
            {
                rectActorSetPositionToMouseLocation(this);
            }
            break;
        }
        case HS_GAME_STATE_OBJECT_CHOOSE_DIMENSION:
        {
            this->ca.frameStart = a->eng->currentFrame;
            if (rectActorIsFocussedActor(this))
            {
                rectActorSetDimensionToMouseLocation(this);
            }
            break;
        }
        case HS_GAME_STATE_OBJECT_CHOOSE_VELOCITY:
        {
            this->ca.frameStart = a->eng->currentFrame;
            if (rectActorIsFocussedActor(this))
            {
                rectActorSetVelocityToMouseLocation(this);
            }
            break;
        }
        case HS_GAME_STATE_CHOOSE_OBJECT:
        {
            this->ca.frameStart = a->eng->currentFrame;
            break;
        }
        default:
        {
            break;
        }
    }
}

static void rectActorSetPositionToMouseLocation(
        rectActor * this)
{
    jint w, h;
    w = jintRectGetWidth(&this->ca.shape.rect);
    h = jintRectGetHeight(&this->ca.shape.rect);

    this->ca.shape.rect.bl = engineGetMouseLocation(this->a.eng);
    this->ca.shape.rect.tr.v[0] = this->ca.shape.rect.bl.v[0] + w;
    this->ca.shape.rect.tr.v[1] = this->ca.shape.rect.bl.v[1] + h;

    this->rectAnchorPoint = this->ca.shape.rect.bl;
}

static void rectActorSetDimensionToMouseLocation(
        rectActor * this)
{
    jintVec mouseCoords = engineGetMouseLocation(this->a.eng);

    jint wh[2] = {mouseCoords.v[0] - this->rectAnchorPoint.v[0],
        mouseCoords.v[1] - this->rectAnchorPoint.v[1]};

    jint axis;
    for (axis = 0; axis < 2; axis++)
    {
        if (wh[axis] >= 0)
        {
            this->ca.shape.rect.tr.v[axis] = this->ca.shape.rect.bl.v[axis] + wh[axis];
        }
        else
        {
            this->ca.shape.rect.tr.v[axis] = this->rectAnchorPoint.v[axis];
            this->ca.shape.rect.bl.v[axis] = mouseCoords.v[axis];
        }
    }
}

static void rectActorSetVelocityToMouseLocation(
        rectActor * this)
{
    jintVec lStart;
    jintVec lEnd = engineGetMouseLocation(this->a.eng);

    lStart = this->ca.shape.rect.bl;
    lStart.v[0] += jintRectGetWidth(&this->ca.shape.rect)/2;
    lStart.v[1] += jintRectGetHeight(&this->ca.shape.rect)/2;

    jintVecScaled v = {.v = jintVecSub(lEnd, lStart), .s = 80};
    rectActorSetVelocity(
            &this->oa, &v);
}

static void rectActorResetAppearence(objectActor * oa)
{
    rectActor * this = oa->ptr.ra;
    this->ca.shape.rect = this->originalRect;
}

static void rectActorSetVelocity(
        objectActor * oa, const jintVecScaled * v)
{
    rectActor * this = oa->ptr.ra;
    collEngineCollActorSetVelocity(
            ((herdingSheepsEngine *)this->a.eng->owner)->collEngine,
            &this->ca, v);
}

static collActor * rectActorGetCollActor(
        const objectActor * oa)
{
    rectActor * this = oa->ptr.ra;

    return &this->ca;
}

static bool rectActorIsFocussedActor(
        const rectActor * this)
{
    herdingSheepsEngine * hsEng = this->a.eng->owner;
    objectActor * focussedActor = hsEng->objectActorList->val;

    if (focussedActor->type != OBJECT_ACTOR_TYPE_RECT)
        return false;

    if (focussedActor->ptr.ra == this)
        return true;

    return false;
}
