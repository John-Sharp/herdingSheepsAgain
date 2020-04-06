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
    int mouse_x, mouse_y;

    SDL_GetMouseState(&mouse_x, &mouse_y);
    this->ca.shape.line.rStart.v[0] = mouse_x;
    this->ca.shape.line.rStart.v[1] = this->a.eng->h - mouse_y;
    this->lineAnchorPoint = this->ca.shape.line.rStart;
}

static void lineActorSetDimensionToMouseLocation(
        lineActor * this)
{
    jint mouseCoords[2];

    SDL_GetMouseState(&mouseCoords[0], &mouseCoords[1]);
    mouseCoords[1] = this->a.eng->h - mouseCoords[1];

    jint coord = this->ca.type == COLL_ACTOR_TYPE_H_LINE ? 0 : 1;

    jint length = mouseCoords[coord] - this->lineAnchorPoint.v[coord];

    if (length < 0)
    {
        this->ca.shape.line.rStart.v[coord] = mouseCoords[coord];
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
    objectActor * focussedActor = hsEng->otherActorList->val;

    if (focussedActor->type != OBJECT_ACTOR_TYPE_V_LINE 
            && focussedActor->type != OBJECT_ACTOR_TYPE_H_LINE)
        return false;

    if (focussedActor->ptr.la == this)
        return true;

    return false;
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
        case HS_GAME_STATE_MAIN_OBJECT_CHOOSE_VELOCITY:
        {
            jintVec lStart, lEnd;
            SDL_GetMouseState(&lEnd.v[0], &lEnd.v[1]);
            lEnd.v[1] = a->eng->h - lEnd.v[1];

            lStart = this->ca.shape.line.rStart;
            jint coordIndex = this->ca.type == COLL_ACTOR_TYPE_H_LINE ? 0 : 1;
            lStart.v[coordIndex] += this->ca.shape.line.length/2;
            this->ca.vel.v = jintVecSub(lEnd, lStart);
            this->ca.vel.s = 80;
            this->ca.frameStart = a->eng->currentFrame;
            break;
        }
        case HS_GAME_STATE_MAIN_OBJECT_CHOOSE_DIMENSION:
        {
            this->ca.frameStart = a->eng->currentFrame;
            lineActorSetDimensionToMouseLocation(this);
            break;
        }
        case HS_GAME_STATE_MAIN_OBJECT_H_LINE:
        case HS_GAME_STATE_MAIN_OBJECT_V_LINE:
        {
            lineActorSetPositionToMouseLocation(this);
            break;
        }
        case HS_GAME_STATE_CHOOSE_OTHER_OBJECT:
        {
            this->ca.frameStart = a->eng->currentFrame;
            break;
        }
        case HS_GAME_STATE_OTHER_OBJECT_BEING_POSITIONED:
        {
            this->ca.frameStart = a->eng->currentFrame;
            if (lineActorIsFocussedActor(this))
            {
                lineActorSetPositionToMouseLocation(this);
            }
            break;
        }
        case HS_GAME_STATE_OTHER_OBJECT_CHOOSE_DIMENSION:
        {
            this->ca.frameStart = a->eng->currentFrame;
            if (lineActorIsFocussedActor(this))
            {
                lineActorSetDimensionToMouseLocation(this);
            }
            break;
        }
        default:
            break;
    }
}

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
        .vel = {.v = {{0, 0}}, .s = 80}
    };
    this->ca = ca;
    this->line = &this->ca.shape.line;

    this->oa.type = params->line.direction == AX_PL_DIR_Y ? \
                    OBJECT_ACTOR_TYPE_V_LINE : OBJECT_ACTOR_TYPE_H_LINE;
    this->oa.ptr.la = this;

    engineActorReg(eng, &this->a);
}

void lineActorDeinit(lineActor * this)
{
    actorEngineDereg(&this->a);
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

    ln->rStart = jintLineGetPosition(&l, frame - this->ca.frameStart);
}
