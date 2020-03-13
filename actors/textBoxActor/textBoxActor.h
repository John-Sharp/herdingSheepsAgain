#ifndef FRAME_RATE_BAR_ACTOR_H
#define FRAME_RATE_BAR_ACTOR_H

#include "../../engine/engine.h"
#include <jTypes.h>

typedef struct textBoxActor textBoxActor;

// function ptr that points to a function implemented by textBoxActor that will
// receive `text` and draw it into the textBox
typedef void (*textReceiver)(const char * text);

// function ptr that points to a function implemented by client code that is
// called by textBoxActor's drawing routine. When called, this function should
// pass the text to be drawn into `tr`
typedef void (*textProvider)(textBoxActor * t, textReceiver tr);

// function ptr that points to a function implemented by client code. Is called
// by textBoxActor every render loop and should return an appropriate
// `textProvider` if and only if there is fresh text to display
typedef textProvider (*textProviderProvider)(textBoxActor * t);

typedef struct textBoxParams {
    jintRect window;
    jcolor fg;
    jcolor bg;
    juint fontSize;
    textProviderProvider hasRefreshedTextFn;
} textBoxParams;

typedef struct textBoxActor {
    actor a;
    sprite s;
    decal d;

    jcolor bgColor;
    jcolor fgColor;
    juint fontSize;

    jintRect window;

    textProviderProvider hasRefreshedText;

    jint prevUpdate;
} textBoxActor;

void initTextBox(engine * eng, textBoxActor * t, const textBoxParams * params);

#endif
