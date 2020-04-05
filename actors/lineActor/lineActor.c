#include "lineActor.h" 

#include "../../herdingSheepsEngine/herdingSheepsEngine.h"

#include "../../herdingSheepsEngine/listCode/lineActorList.inc"

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

void lineActorLogicHandler(actor * a)
{
    lineActor * la = a->owner;
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

            lStart = la->ca.shape.line.rStart;
            jint coordIndex = la->ca.type == COLL_ACTOR_TYPE_H_LINE ? 0 : 1;
            lStart.v[coordIndex] += la->ca.shape.line.length/2;
            la->ca.vel.v = jintVecSub(lEnd, lStart);
            la->ca.vel.s = 80;
            la->ca.frameStart = a->eng->currentFrame;
            break;
        }
        case HS_GAME_STATE_MAIN_OBJECT_CHOOSE_DIMENSION:
        {
            jint mouseCoords[2];

            SDL_GetMouseState(&mouseCoords[0], &mouseCoords[1]);
            mouseCoords[1] = a->eng->h - mouseCoords[1];

            jint coord = la->ca.type == COLL_ACTOR_TYPE_H_LINE ? 0 : 1;

            jint length = mouseCoords[coord] - la->lineAnchorPoint.v[coord];

            if (length < 0)
            {
                la->ca.shape.line.rStart.v[coord] = mouseCoords[coord];
                la->ca.shape.line.length = -length;
            }
            else
            {
                la->ca.shape.line.length = length;
            }
            la->ca.frameStart = a->eng->currentFrame;

            break;
        }
        case HS_GAME_STATE_MAIN_OBJECT_H_LINE:
        case HS_GAME_STATE_MAIN_OBJECT_V_LINE:
        {
            int mouse_x, mouse_y;

            SDL_GetMouseState(&mouse_x, &mouse_y);
            la->ca.shape.line.rStart.v[0] = mouse_x;
            la->ca.shape.line.rStart.v[1] = a->eng->h - mouse_y;
            la->lineAnchorPoint = la->ca.shape.line.rStart;
            break;
        }
        case HS_GAME_STATE_CHOOSE_OTHER_OBJECT:
        {
            la->ca.frameStart = a->eng->currentFrame;
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

    this->ma.type = params->line.direction == AX_PL_DIR_Y ? \
                    MAIN_ACTOR_TYPE_V_LINE : MAIN_ACTOR_TYPE_H_LINE;
    this->ma.ptr.la = this;

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
