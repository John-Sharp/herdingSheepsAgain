#ifndef LINE_ACTOR_H
#define LINE_ACTOR_H

#include "../../engine/engine.h"
#include "../objectActor/objectActor.h"
#include <jTypes.h>
#include <collDetect.h>

typedef struct herdingSheepsEngine herdingSheepsEngine;

typedef struct lineActorParams {
    jintAxPlLine line;
} lineActorParams;

typedef struct lineActor {
    actor a;

    collActor ca;
    jintAxPlLine originalLine;

    objectActor oa;

    jintVec lineAnchorPoint;
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
        jintAxPlLine * ln);

void lineActorGetLineAtFrame(
        const lineActor * this,
        jint frame, 
        jintAxPlLine * ln);
#endif
