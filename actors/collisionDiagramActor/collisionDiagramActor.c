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

    // draw blue Point
    jintVec r;
    movingPointActorGetPosition(&e->bluePoint, &r.x, &r.y, e->engine->currentFrame);
    cairo_arc (cr, r.x, (600-25) - r.y, 30, 0, 2 * M_PI);
	cairo_fill (cr);

    // draw wall
	cairo_set_source_rgb (cr, 1, 0, 1);
    cairo_move_to(cr, e->wall.pos.x, (600-25) - e->wall.pos.y);
    cairo_rel_line_to (cr, 0, -e->wall.length);
    cairo_stroke(cr);

    if (e->bluePoint.vLine.sTarg.x != 0 || e->bluePoint.vLine.sTarg.x != 0)
    {
        // draw extended velocity vector
        cairo_set_source_rgb (cr, 1, 1, 0);
        cairo_move_to(cr, e->bluePoint.vLine.rStart.x, (600-25) - e->bluePoint.vLine.rStart.y);
        jint u, v;

        if (e->bluePoint.vLine.sTarg.x > e->bluePoint.vLine.rStart.x)
        {
            u = e->bluePoint.vLine.sTarg.x ? (800 - e->bluePoint.vLine.rStart.x) / e->bluePoint.vLine.sTarg.x + 1 : 0;
        }
        else
        {
            u = e->bluePoint.vLine.sTarg.x ? -1 * (e->bluePoint.vLine.rStart.x) / e->bluePoint.vLine.sTarg.x + 1 : 0;
        }

        if (e->bluePoint.vLine.sTarg.y > e->bluePoint.vLine.rStart.y)
        {
            v = e->bluePoint.vLine.sTarg.y ? ((600 - 25) - e->bluePoint.vLine.rStart.y) / e->bluePoint.vLine.sTarg.y + 1 : 0;
        }
        else
        {
            v = e->bluePoint.vLine.sTarg.y ? -1 * (e->bluePoint.vLine.rStart.y) / e->bluePoint.vLine.sTarg.y + 1 : 0;
        }

        u = u > v ? u : v;
        cairo_rel_line_to (cr, u * e->bluePoint.vLine.sTarg.x, -u * e->bluePoint.vLine.sTarg.y);
        cairo_stroke(cr);

        //draw extended tick marks
        cairo_set_source_rgb (cr, 1, 1, 0);
        int i;
        for (i = 10; ; i+=10)
        {
            int tickx = i * e->bluePoint.vLine.sTarg.x / e->bluePoint.vLine.tScale + e->bluePoint.vLine.rStart.x;
            int ticky = i * e->bluePoint.vLine.sTarg.y / e->bluePoint.vLine.tScale + e->bluePoint.vLine.rStart.y;

            if (tickx > 800+5 || tickx < 0)
                break;

            if (ticky > 600-25+5 || ticky < 0)
                break;

            cairo_arc (cr, tickx, (600-25) - ticky, 5, 0, 2 * M_PI);
            cairo_fill (cr);
        }

        // draw velocity vector
        cairo_set_source_rgb (cr, 0, 1, 0);
        cairo_move_to(cr, e->bluePoint.vLine.rStart.x, (600-25) - e->bluePoint.vLine.rStart.y);
        cairo_rel_line_to (cr, e->bluePoint.vLine.sTarg.x, -e->bluePoint.vLine.sTarg.y);
        cairo_stroke(cr);

        // draw tickmarks
        cairo_set_source_rgb (cr, 0, 1, 0);
        for (i = 10; i <=  e->bluePoint.vLine.tScale; i+=10)
        {
            int tickx = i * e->bluePoint.vLine.sTarg.x / e->bluePoint.vLine.tScale + e->bluePoint.vLine.rStart.x;
            int ticky = i * e->bluePoint.vLine.sTarg.y / e->bluePoint.vLine.tScale + e->bluePoint.vLine.rStart.y;

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
