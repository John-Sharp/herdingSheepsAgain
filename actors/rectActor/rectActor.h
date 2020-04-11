#ifndef RECT_ACTOR_H
#define RECT_ACTOR_H

#include "../../engine/engine.h"
#include "../objectActor/objectActor.h"
#include <jTypes.h>
#include <collDetect.h>

typedef struct rectActorParams {
    jintRect rect;
} rectActorParams;

typedef struct rectActor {
    actor a;

    collActor ca;

    objectActor oa;
    jintRect originalRect;

    jintVec rectAnchorPoint;
} rectActor;

rectActor * createRectActor(
        engine * eng,
        const rectActorParams * params);

void rectActorInit(
        rectActor * this,
        engine * eng,
        const rectActorParams * params);

void rectActorDeinit(rectActor * this);

void rectActorGetRect(
        const rectActor * this,
        jintRect * rect);

void rectActorGetRectAtFrame(
        const rectActor * this,
        jint frame,
        jintRect * rect);
#endif
