#ifndef LINE_ACTOR_H
#define LINE_ACTOR_H

#include "../../engine/engine.h"
#include <jTypes.h>
#include <collDetect.h>

typedef struct herdingSheepsEngine herdingSheepsEngine;

typedef struct lineActorParams {
    jintAxPlLine line;
} lineActorParams;

typedef struct lineActor {
    actor a;

    collActor ca;
    const jintAxPlLine * line;

    juint frameStart;
} lineActor;

lineActor * createLineActor(
        engine * eng,
        const lineActorParams * params);

void lineActorInit(
        lineActor * this,
        engine * eng,
        const lineActorParams * params);

void lineActorDeinit(lineActor * this);

void lineActorGetLine(
        const lineActor * this,
        juint frame,
        jintAxPlLine * ln);

#endif
