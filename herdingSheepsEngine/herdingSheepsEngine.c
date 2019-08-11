#include "herdingSheepsEngine.h"

void herdingSheepsPreLogic(engine * e)
{
}

void testCB(jint x, jint y, void * owner)
{
    printf("got mouse event at %d %d\n", x, y);
}

herdingSheepsEngine * initHerdingSheepsEngine(herdingSheepsEngine * eng)
{
    eng->engine = createEngine(800, 600, eng);

    enginePreLogicCallBackReg(eng->engine, herdingSheepsPreLogic);

    // setup collisionDiagram
    initCollisionDiagram(eng->engine, &eng->collisionDiagram);

    // setup frameRateBar
    initFrameRateBar(eng->engine, &eng->frameRateBar);

    // setup bluePoint
    initMovingPointActor(eng->engine, &eng->bluePoint);


    mouseCallbackBinding mouseBinding = {
        .type = SDL_MOUSEBUTTONDOWN,
        .button = SDL_BUTTON_LEFT,
        .callback = testCB,
        .owner = NULL
    };
    addMouseCallback(&mouseBinding);

    return eng;
}

