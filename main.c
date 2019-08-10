#include "engine/engine.h"
#include <jTypes.h>
#include <SDL2/SDL.h>

#include <cairo.h>
#include <math.h>

#include <stdio.h>

typedef struct frameRateBarActor {
    actor a;
    sprite s;
    decal d;
} frameRateBarActor;

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
    frameRateBarActor frameRateBar;

} herdingSheepsEngine;

void herdingSheepsPreLogic(engine * e)
{
}

void redCircleRenderer(actor * a)
{
    redCircleActor * c = (redCircleActor *)a->owner;

    engineSpriteRender(a->eng, &c->s);
}

void drawBlueCircle(void * pixels, int pitch, void * ctx)
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

	cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_move_to(cr, 0,100);
    cairo_set_font_size(cr, 20);
    cairo_show_text(cr, "hello world");
}

void collisionDiagramRenderer(actor * a)
{
    collisionDiagramActor * c = (collisionDiagramActor *)a->owner;

    engineUpdateTexturesPixels(a->eng, c->d.textureId, drawBlueCircle, NULL);

    engineSpriteRender(a->eng, &c->s);
}

void drawFrameRateBar(void * pixels, int pitch, void * ctx)
{
    frameRateBarActor * f = (frameRateBarActor *)ctx;
    memset(pixels, 0, 100 * pitch);

    cairo_surface_t *cairosurf = cairo_image_surface_create_for_data (
            pixels,
            CAIRO_FORMAT_ARGB32,
            800,
            25,
            pitch);

    cairo_t * cr = cairo_create (cairosurf);

	cairo_set_source_rgb (cr, 1, 0, 0);

	cairo_paint (cr);

	cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_move_to(cr, 4,18);
    cairo_set_font_size(cr, 15);

    char frameRateInfoBuffer[256];
    uint32_t renderFPS, logicFPS;
    engineGetFrameRate(f->a.eng, &logicFPS, &renderFPS);
    sprintf(frameRateInfoBuffer, "render FPS: %u, logic FPS: %u", renderFPS, logicFPS);
    cairo_show_text(cr, frameRateInfoBuffer);
}

void frameRateBarRenderer(actor * a)
{
    frameRateBarActor * f = (frameRateBarActor *)a->owner;
    static int prevUpdate = 0;

    int update = SDL_GetTicks() / 500;
    if (update > prevUpdate)
    {
        prevUpdate = update;
        engineUpdateTexturesPixels(a->eng, f->d.textureId, drawFrameRateBar, f);
    }

    engineSpriteRender(a->eng, &f->s);
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

    // setup frameRateBar
    eng->frameRateBar.a.owner = &eng->frameRateBar;
    eng->frameRateBar.a.renderHandler = frameRateBarRenderer;
    eng->collisionDiagram.a.logicHandler = NULL;
    juint frameRateBarTexture = engineCreateTexture(
            eng->engine, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 800, 100);
    {
        jintRect r = {.bl= {0,0}, .tr= {800, 100}}; 
        decalInit(&eng->frameRateBar.d, eng->engine, frameRateBarTexture, r);
    }
    eng->frameRateBar.s.d = &eng->frameRateBar.d;
    {
        jintRect r = {.bl= {0,500}, .tr= {800, 600}}; 
        eng->frameRateBar.s.rect = r;
    }
    engineActorReg(eng->engine, &eng->frameRateBar.a);

    engineActorReg(eng->engine, &eng->redCircle.a);

    return eng;
}

int main()
{
    herdingSheepsEngine eng;
    initHerdingSheepsEngine(&eng);

    engineStart(eng.engine);
}
