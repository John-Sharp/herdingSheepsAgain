#include "engine/engine.h"
#include <jTypes.h>
#include <SDL2/SDL.h>

#include <cairo.h>
#include <math.h>

#include <stdio.h>

typedef struct redCircleActor {
    actor a;
    sprite s;
} redCircleActor;

typedef struct collisionDiagramActor {
    actor a;
    sprite s;
    decal d;
} collisionDiagramActor;

typedef struct herdingSheepsEngine {
    engine * engine;

    decal circleDecal;
    redCircleActor redCircle;

    collisionDiagramActor collisionDiagram;

} herdingSheepsEngine;

void herdingSheepsPreLogic(engine * e)
{
}

void redCircleRenderer(actor * a)
{
    redCircleActor * c = (redCircleActor *)a->owner;

    engineSpriteRender(a->eng, &c->s);
}

void drawBlueCircle(void * pixels, int pitch)
{
    memset(pixels, 0, 200 * pitch);

    cairo_surface_t *cairosurf = cairo_image_surface_create_for_data (
            pixels,
            CAIRO_FORMAT_ARGB32,
            200,
            200,
            pitch);

    cairo_t * cr = cairo_create (cairosurf);

	cairo_set_source_rgb (cr, 0, 0, 1);

    cairo_arc (cr, 100, 100, 30, 0, 2 * M_PI);

	cairo_fill (cr);
}

void collisionDiagramRenderer(actor * a)
{
    collisionDiagramActor * c = (collisionDiagramActor *)a->owner;

    engineUpdateTexturesPixels(a->eng, c->d.textureId, drawBlueCircle);

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
    jintRect s = {.bl={0,0}, .tr={100, 100}};
    eng->redCircle.s.rect = s;

    // setup collisionDiagram
    eng->collisionDiagram.a.owner = &eng->collisionDiagram;
    eng->collisionDiagram.a.renderHandler = collisionDiagramRenderer;
    eng->collisionDiagram.a.logicHandler = NULL;
    juint collisionDiagramTexture = engineCreateTexture(
            eng->engine, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 200, 200);
    {
        jintRect r = {.bl= {0,0}, .tr= {200, 200}}; 
        decalInit(&eng->collisionDiagram.d, eng->engine, collisionDiagramTexture, r);
    }
    eng->collisionDiagram.s.d = &eng->collisionDiagram.d;
    {
        jintRect r = {.bl= {0,0}, .tr= {200, 200}}; 
        eng->collisionDiagram.s.rect = r;
    }
    engineActorReg(eng->engine, &eng->collisionDiagram.a);

    engineActorReg(eng->engine, &eng->redCircle.a);

    return eng;
}

int main()
{
    herdingSheepsEngine eng;
    initHerdingSheepsEngine(&eng);

    engineStart(eng.engine);
}
