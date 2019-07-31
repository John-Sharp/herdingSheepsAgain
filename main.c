#include "engine/engine.h"
#include <jTypes.h>

#include <stdio.h>

typedef struct redCircleActor {
    actor a;
    sprite s;
} redCircleActor;

typedef struct herdingSheepsEngine {
    engine * engine;

    decal circleDecal;
    redCircleActor redCircle;
} herdingSheepsEngine;

void herdingSheepsPreLogic(engine * e)
{
}

void redCircleRenderer(actor * a)
{
    redCircleActor * c = (redCircleActor *)a->owner;

    engineSpriteRender(a->eng, &c->s);
}

herdingSheepsEngine * initHerdingSheepsEngine(herdingSheepsEngine * eng)
{
    eng->engine = createEngine(800, 600, eng);

    juint circleTexture = engineLoadTexture(eng->engine, "assets/imgs/red_circle.png");

    enginePreLogicCallBackReg(eng->engine, herdingSheepsPreLogic);

    jintRect r = {.bl= {0,0}, .tr= {100, 100}};
    decalInit(&eng->circleDecal, eng->engine, circleTexture, r);

    eng->redCircle.a.owner = &eng->redCircle;
    eng->redCircle.a.renderHandler = redCircleRenderer;
    eng->redCircle.a.logicHandler = NULL;
    eng->redCircle.s.d = &eng->circleDecal;
    jintRect s = {.bl={0,0}, .tr={20, 20}};
    eng->redCircle.s.rect = s;
    engineActorReg(eng->engine, &eng->redCircle.a);

    return eng;
}

int main()
{
    herdingSheepsEngine eng;
    initHerdingSheepsEngine(&eng);

    engineStart(eng.engine);
}
