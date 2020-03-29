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

void lineActorInit(
        lineActor * this,
        engine * eng,
        const lineActorParams * params)
{
    this->a.owner = this;
    this->a.renderHandler = NULL;
    this->a.logicHandler = NULL;

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
}

void lineActorDeinit()
{
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
