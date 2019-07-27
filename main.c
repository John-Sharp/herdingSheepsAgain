#include "engine/engine.h"

#include <stdio.h>

void herdingSheepsPreLogic(engine * e)
{
    printf("pre-logic handler being called\n");
}

typedef struct herdingSheepsEngine {
    engine * engine;
} herdingSheepsEngine;

herdingSheepsEngine * initHerdingSheepsEngine(herdingSheepsEngine * eng)
{
    eng->engine = createEngine(800, 600, eng);

    enginePreLogicCallBackReg(eng->engine, herdingSheepsPreLogic);

    return eng;
}

int main()
{
    herdingSheepsEngine eng;
    initHerdingSheepsEngine(&eng);

    engineStart(eng.engine);
}
