#ifndef FRAME_RATE_BAR_ACTOR_H
#define FRAME_RATE_BAR_ACTOR_H

#include "../../engine/engine.h"
#include <jTypes.h>

typedef struct frameRateBarActor {
    actor a;
    sprite s;
    decal d;

    jintRect window;
} frameRateBarActor;

void initFrameRateBar(engine * eng, frameRateBarActor * f);

#endif
