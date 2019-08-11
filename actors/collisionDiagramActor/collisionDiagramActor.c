#include "collisionDiagramActor.h" 

#include <SDL2/SDL.h>

#include <cairo.h>

#include "../../engine/engine.h"
#include "../../herdingSheepsEngine/herdingSheepsEngine.h"
#include <jTypes.h>

void drawCollisionDiagram(void * pixels, int pitch, void * ctx)
{
    herdingSheepsEngine * e = ((engine *)ctx)->owner;
    memset(pixels, 0, (600 - 25) * pitch);

    cairo_surface_t *cairosurf = cairo_image_surface_create_for_data (
            pixels,
            CAIRO_FORMAT_ARGB32,
            800,
            600 - 25,
            pitch);

    cairo_t * cr = cairo_create (cairosurf);

	cairo_set_source_rgb (cr, 0, 0, 1);

    jintVec r;
    movingPointActorGetPosition(&e->bluePoint, &r[0], &r[1], e->engine->currentFrame);
    cairo_arc (cr, r[0], (600-25) - r[1], 30, 0, 2 * M_PI);

	cairo_fill (cr);

    if (e->bluePoint.sTarg[0] != 0 || e->bluePoint.sTarg[0] != 0)
    {
        // draw extended velocity vector
        cairo_set_source_rgb (cr, 1, 1, 0);
        cairo_move_to(cr, e->bluePoint.rStart[0], (600-25) - e->bluePoint.rStart[1]);
        jint u, v;

        if (e->bluePoint.sTarg[0] > e->bluePoint.rStart[0])
        {
            u = e->bluePoint.sTarg[0] ? (800 - e->bluePoint.rStart[0]) / e->bluePoint.sTarg[0] + 1 : 0;
        }
        else
        {
            u = e->bluePoint.sTarg[0] ? -1 * (e->bluePoint.rStart[0]) / e->bluePoint.sTarg[0] + 1 : 0;
        }

        if (e->bluePoint.sTarg[1] > e->bluePoint.rStart[1])
        {
            v = e->bluePoint.sTarg[1] ? ((600 - 25) - e->bluePoint.rStart[1]) / e->bluePoint.sTarg[1] + 1 : 0;
        }
        else
        {
            v = e->bluePoint.sTarg[1] ? -1 * (e->bluePoint.rStart[1]) / e->bluePoint.sTarg[1] + 1 : 0;
        }

        u = u > v ? u : v;
        cairo_rel_line_to (cr, u * e->bluePoint.sTarg[0], -u * e->bluePoint.sTarg[1]);
        cairo_stroke(cr);

        //draw extended tick marks
        cairo_set_source_rgb (cr, 1, 1, 0);
        int i;
        for (i = 10; ; i+=10)
        {
            int tickx = i * e->bluePoint.sTarg[0] / e->bluePoint.targFrame + e->bluePoint.rStart[0];
            int ticky = i * e->bluePoint.sTarg[1] / e->bluePoint.targFrame + e->bluePoint.rStart[1];

            if (tickx > 800+5 || tickx < 0)
                break;

            if (ticky > 600-25+5 || ticky < 0)
                break;

            cairo_arc (cr, tickx, (600-25) - ticky, 5, 0, 2 * M_PI);
            cairo_fill (cr);
        }

        // draw velocity vector
        cairo_set_source_rgb (cr, 0, 1, 0);
        cairo_move_to(cr, e->bluePoint.rStart[0], (600-25) - e->bluePoint.rStart[1]);
        cairo_rel_line_to (cr, e->bluePoint.sTarg[0], -e->bluePoint.sTarg[1]);
        cairo_stroke(cr);

        // draw tickmarks
        cairo_set_source_rgb (cr, 0, 1, 0);
        for (i = 10; i <=  e->bluePoint.targFrame; i+=10)
        {
            int tickx = i * e->bluePoint.sTarg[0] / e->bluePoint.targFrame + e->bluePoint.rStart[0];
            int ticky = i * e->bluePoint.sTarg[1] / e->bluePoint.targFrame + e->bluePoint.rStart[1];

            cairo_arc (cr, tickx, (600-25) - ticky, 5, 0, 2 * M_PI);
            cairo_fill (cr);
        }
    }
}

void collisionDiagramRenderer(actor * a)
{
    collisionDiagramActor * c = (collisionDiagramActor *)a->owner;

    engineUpdateTexturesPixels(a->eng, c->d.textureId, drawCollisionDiagram, a->eng);

    engineSpriteRender(a->eng, &c->s);
}

void initCollisionDiagram(engine * eng, collisionDiagramActor * c)
{
    c->a.owner = c;
    c->a.renderHandler = collisionDiagramRenderer;
    c->a.logicHandler = NULL;
    juint collisionDiagramTexture = engineCreateTexture(
            eng, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 800, 600 - 25);
    decalInit(&c->d, eng, collisionDiagramTexture, createJintRect(0, 0, 800, 600 - 25));
    c->s.d = &c->d;
    c->s.rect = createJintRect(0, 0, 800, 600 -25);
    engineActorReg(eng, &c->a);
}
