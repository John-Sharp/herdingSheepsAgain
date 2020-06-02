#include "lineActor.h" 

#include "../../herdingSheepsEngine/herdingSheepsEngine.h"

lineActor * createLineActor(
        engine * eng,
        const lineActorParams * params)
{
    lineActor * ret;

    ret = malloc(sizeof(*ret));

    if (!ret)
        return ret;

    lineActorInit(ret, eng, params);

    return ret;
}

static void lineActorSetPositionToMouseLocation(
        lineActor * this)
{
    this->ca.shape.line.rStart = engineGetMouseLocation(this->a.eng);
    this->lineAnchorPoint = this->ca.shape.line.rStart;
}

static void lineActorSetDimensionToMouseLocation(
        lineActor * this)
{
    jintVec mouseCoords = engineGetMouseLocation(this->a.eng);

    jint coord = this->ca.type == COLL_ACTOR_TYPE_H_LINE ? 0 : 1;

    jint length = mouseCoords.v[coord] - this->lineAnchorPoint.v[coord];

    if (length < 0)
    {
        this->ca.shape.line.rStart.v[coord] = mouseCoords.v[coord];
        this->ca.shape.line.length = -length;
    }
    else
    {
        this->ca.shape.line.length = length;
    }
}

static bool lineActorIsFocussedActor(
        const lineActor * this)
{
    herdingSheepsEngine * hsEng = this->a.eng->owner;
    objectActor * focussedActor = hsEng->objectActorList->val;

    if (focussedActor->type != OBJECT_ACTOR_TYPE_V_LINE 
            && focussedActor->type != OBJECT_ACTOR_TYPE_H_LINE)
        return false;

    if (focussedActor->ptr.la == this)
        return true;

    return false;
}
   
void lineActorSetVelocity(
        objectActor * oa, const jintVecScaled * v)
{
    lineActor * this = oa->ptr.la;
    collEngineCollActorSetVelocity(
            ((herdingSheepsEngine *)this->a.eng->owner)->collEngine,
            &this->ca, v);
}

void lineActorLogicHandler(actor * a)
{
    lineActor * this = a->owner;
    herdingSheepsEngine * hsEng = a->eng->owner;

    HS_GAME_STATE currentState;
    SBStateMachineGetCurrentState(
        hsEng->mainStateMachine, &currentState);
    switch (currentState)
    {
        case HS_GAME_STATE_CHOOSE_OBJECT:
        {
            this->ca.frameStart = a->eng->currentFrame;
            break;
        }
        case HS_GAME_STATE_OBJECT_BEING_POSITIONED:
        {
            this->ca.frameStart = a->eng->currentFrame;
            if (lineActorIsFocussedActor(this))
            {
                lineActorSetPositionToMouseLocation(this);
            }
            break;
        }
        case HS_GAME_STATE_OBJECT_CHOOSE_DIMENSION:
        {
            this->ca.frameStart = a->eng->currentFrame;
            if (lineActorIsFocussedActor(this))
            {
                lineActorSetDimensionToMouseLocation(this);
            }
            break;
        }
        case HS_GAME_STATE_OBJECT_CHOOSE_VELOCITY:
        {
            this->ca.frameStart = a->eng->currentFrame;
            if (lineActorIsFocussedActor(this))
            {
                jintVec lStart;
                jintVec lEnd = engineGetMouseLocation(this->a.eng);

                lStart = this->ca.shape.line.rStart;
                jint coordIndex = this->ca.type == COLL_ACTOR_TYPE_H_LINE ? 0 : 1;
                lStart.v[coordIndex] += this->ca.shape.line.length/2;
                this->ca.frameStart = a->eng->currentFrame;
                jintVecScaled v = {.v = jintVecSub(lEnd, lStart), .s = 80};
                lineActorSetVelocity(&this->oa, &v);
            }
            break;
        }
        default:
            break;
    }
}

collActor * lineActorGetCollActor(
        const objectActor * oa)
{
    lineActor * this = oa->ptr.la;

    return &this->ca;
}

static void lineActorResetAppearence(objectActor * oa);
void lineActorInit(
        lineActor * this,
        engine * eng,
        const lineActorParams * params)
{
    this->a.owner = this;
    this->a.renderHandler = NULL;
    this->a.logicHandler = lineActorLogicHandler;

    collActor ca = {
        .type = params->line.direction == AX_PL_DIR_Y ? COLL_ACTOR_TYPE_V_LINE : COLL_ACTOR_TYPE_H_LINE,
        .shape = {
            .line = params->line 
        },
        .vel = {.v = {{0, 0}}, .s = 80},
        .categoryNumber = HS_COLLISION_CATEGORY,
        .nextScheduledCollision = NULL
    };
    this->ca = ca;
    collEngineRegisterCollActor(
            ((herdingSheepsEngine *)eng->owner)->collEngine,
            &this->ca);

    this->originalLine = this->ca.shape.line;

    this->oa.type = params->line.direction == AX_PL_DIR_Y ? \
                    OBJECT_ACTOR_TYPE_V_LINE : OBJECT_ACTOR_TYPE_H_LINE;
    this->oa.ptr.la = this;
    this->oa.objectActorSetVelocity = lineActorSetVelocity;
    this->oa.objectActorGetCollActor = lineActorGetCollActor;
    this->oa.objectActorResetAppearence = lineActorResetAppearence;

    engineActorReg(eng, &this->a);
}

void lineActorDeinit(lineActor * this)
{
    actorEngineDereg(&this->a);
    collEngineDeregisterCollActor(
            ((herdingSheepsEngine *)this->a.eng->owner)->collEngine,
            &this->ca);
}

void lineActorGetLine(
        const lineActor * this,
        jintAxPlLine * ln)
{
    *ln = this->ca.shape.line;
    jintLine l = {
        .rStart = this->ca.shape.line.rStart,
        .sTarg = this->ca.vel,
    };

    ln->rStart = jintLineGetPosition(&l, (jint)this->a.eng->currentFrame - this->ca.frameStart);
}

void lineActorGetLineAtFrame(
        const lineActor * this,
        jint frame, 
        jintAxPlLine * ln)
{
    *ln = this->ca.shape.line;
    jintLine l = {
        .rStart = this->ca.shape.line.rStart,
        .sTarg = this->ca.vel,
    };

    ln->rStart = jintLineGetPosition(&l, frame);
}

static void lineActorResetAppearence(objectActor * oa)
{
    lineActor * this = oa->ptr.la;

    this->ca.shape.line = this->originalLine;
}
