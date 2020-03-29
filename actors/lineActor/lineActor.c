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
            // TODO
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

            break;
        }
        default:
        {
            int mouse_x, mouse_y;

            SDL_GetMouseState(&mouse_x, &mouse_y);
            la->ca.shape.line.rStart.v[0] = mouse_x;
            la->ca.shape.line.rStart.v[1] = a->eng->h - mouse_y;
            la->lineAnchorPoint = la->ca.shape.line.rStart;
            break;
        }
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

    this->frameStart = 0;

    collActor ca = {
        .type = params->line.direction == AX_PL_DIR_Y ? COLL_ACTOR_TYPE_V_LINE : COLL_ACTOR_TYPE_H_LINE,
        .shape = {
            .line = params->line 
        },
        .vel = {.v = {0, 0}, .scale = 1}
    };
    this->ca = ca;
    this->line = &this->ca.shape.line;

    engineActorReg(eng, &this->a);
}

void lineActorDeinit(lineActor * this)
{
    actorEngineDereg(&this->a);
}

void lineActorGetLine(
        const lineActor * this,
        juint frame,
        jintAxPlLine * ln)
{
    *ln = this->ca.shape.line;
    jintLine l = {
        .rStart = this->ca.shape.line.rStart,
        .sTarg = this->ca.vel,
        .tScale = this->ca.vel.scale
    };

    ln->rStart = jintLineGetPosition(&l, (jint)frame - (jint)this->frameStart);
}
