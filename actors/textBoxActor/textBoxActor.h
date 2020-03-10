#ifndef FRAME_RATE_BAR_ACTOR_H
#define FRAME_RATE_BAR_ACTOR_H

#include "../../engine/engine.h"
#include <jTypes.h>

typedef struct textBoxActor textBoxActor;
typedef void (*textReceiver)(const char * text);
typedef void (*textProvider)(textBoxActor * t, textReceiver tr);

typedef struct textBoxParams {
    jintRect window;
    jcolor fg;
    jcolor bg;
    juint fontSize;
    textProvider textProvider;
} textBoxParams;

typedef struct textBoxActor {
    actor a;
    sprite s;
    decal d;

    jcolor bgColor;
    jcolor fgColor;
    juint fontSize;

    jintRect window;

    textProvider getTextBoxText;
} textBoxActor;

void initTextBox(engine * eng, textBoxActor * t, const textBoxParams * params);

#endif
