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

    // draw mainActor
    if (e->mainActor.type == MAIN_ACTOR_TYPE_UNSET)
    {
        // do nothing
    }
    // jintVec r;
    // movingPointActorGetPosition(&e->bluePoint, &r.v[0], &r.v[1], e->engine->currentFrame);
    // cairo_arc (cr, r.v[0], (600-25) - r.v[1], 30, 0, 2 * M_PI);
	// cairo_fill (cr);

    // draw walls
    {
    wallActorList * wallActorNode = NULL;
    for (wallActorNode = e->walls; wallActorNode != NULL; wallActorNode = wallActorNode->next)
    {
        wallActor * wall = wallActorNode->val;
        cairo_set_source_rgb (cr, 1, 0, 1);
        cairo_move_to(cr, wall->line->rStart.v[0], (600-25) - wall->line->rStart.v[1]);
        if (wall->line->direction == AX_PL_DIR_Y)
        {
            cairo_rel_line_to (cr, 0, -wall->line->length);
        }
        else
        {
            cairo_rel_line_to (cr, wall->line->length, 0);
        }
        cairo_stroke(cr);
    }
    }

    // jintLine bluePointVelLine = {
    //     .rStart = e->bluePoint.ca.shape.point,
    //     .sTarg = e->bluePoint.ca.vel,
    //     .tScale = e->bluePoint.ca.vel.scale
    // };
    // if (bluePointVelLine.sTarg.v[0] != 0 || bluePointVelLine.sTarg.v[1] != 0)
    // {
    //     // draw extended velocity vector
    //     cairo_set_source_rgb (cr, 1, 1, 0);
    //     cairo_move_to(cr, bluePointVelLine.rStart.v[0], (600-25) - bluePointVelLine.rStart.v[1]);
    //     jint u, v;

    //     if (bluePointVelLine.sTarg.v[0] > bluePointVelLine.rStart.v[0])
    //     {
    //         u = bluePointVelLine.sTarg.v[0] ? (800 - bluePointVelLine.rStart.v[0]) / bluePointVelLine.sTarg.v[0] + 1 : 0;
    //     }
    //     else
    //     {
    //         u = bluePointVelLine.sTarg.v[0] ? -1 * (bluePointVelLine.rStart.v[0]) / bluePointVelLine.sTarg.v[0] + 1 : 0;
    //     }

    //     if (bluePointVelLine.sTarg.v[1] > bluePointVelLine.rStart.v[1])
    //     {
    //         v = bluePointVelLine.sTarg.v[1] ? ((600 - 25) - bluePointVelLine.rStart.v[1]) / bluePointVelLine.sTarg.v[1] + 1 : 0;
    //     }
    //     else
    //     {
    //         v = bluePointVelLine.sTarg.v[1] ? -1 * (bluePointVelLine.rStart.v[1]) / bluePointVelLine.sTarg.v[1] + 1 : 0;
    //     }

    //     u = u > v ? u : v;
    //     cairo_rel_line_to (cr, u * bluePointVelLine.sTarg.v[0], -u * bluePointVelLine.sTarg.v[1]);
    //     cairo_stroke(cr);

    //     //draw extended tick marks
    //     cairo_set_source_rgb (cr, 1, 1, 0);
    //     int i;
    //     for (i = 10; ; i+=10)
    //     {
    //         int tickx = i * bluePointVelLine.sTarg.v[0] / bluePointVelLine.tScale + bluePointVelLine.rStart.v[0];
    //         int ticky = i * bluePointVelLine.sTarg.v[1] / bluePointVelLine.tScale + bluePointVelLine.rStart.v[1];

    //         if (tickx > 800+5 || tickx < 0)
    //             break;

    //         if (ticky > 600-25+5 || ticky < 0)
    //             break;

    //         cairo_arc (cr, tickx, (600-25) - ticky, 5, 0, 2 * M_PI);
    //         cairo_fill (cr);
    //     }

    //     // draw velocity vector
    //     cairo_set_source_rgb (cr, 0, 1, 0);
    //     cairo_move_to(cr, bluePointVelLine.rStart.v[0], (600-25) - bluePointVelLine.rStart.v[1]);
    //     cairo_rel_line_to (cr, bluePointVelLine.sTarg.v[0], -bluePointVelLine.sTarg.v[1]);
    //     cairo_stroke(cr);

    //     // draw tickmarks
    //     cairo_set_source_rgb (cr, 0, 1, 0);
    //     for (i = 10; i <=  bluePointVelLine.tScale; i+=10)
    //     {
    //         int tickx = i * bluePointVelLine.sTarg.v[0] / bluePointVelLine.tScale + bluePointVelLine.rStart.v[0];
    //         int ticky = i * bluePointVelLine.sTarg.v[1] / bluePointVelLine.tScale + bluePointVelLine.rStart.v[1];

    //         cairo_arc (cr, tickx, (600-25) - ticky, 5, 0, 2 * M_PI);
    //         cairo_fill (cr);
    //     }

    //     // draw collision point(s)
    //     {
    //         wallActorList * wallActorNode = NULL;
    //         for (wallActorNode = e->walls; wallActorNode != NULL; wallActorNode = wallActorNode->next)
    //         {
    //             wallActor * wall = wallActorNode->val;
    //             if (e->engine->currentFrame < wall->ca.collFrame + e->bluePoint.frameStart)
    //             {
    //                 cairo_set_source_rgb (cr, 1, 0.6, 0);
    //             }
    //             else
    //             {
    //                 cairo_set_source_rgb (cr, 1, 0.0, 0);
    //             }
    //             jintVec collision_point = jintLineGetPosition(&bluePointVelLine, wall->ca.collFrame);
    //             cairo_arc (cr, collision_point.v[0], (600-25) - collision_point.v[1], 3, 0, 2 * M_PI);
    //             cairo_fill (cr);

    //         }
    //     }

    //     if (e->bluePoint.ca.collFrame >= 0)
    //     {
    //         if (e->engine->currentFrame < e->bluePoint.ca.collFrame + e->bluePoint.frameStart)
    //         {
    //             cairo_set_source_rgb (cr, 1, 0.6, 0);
    //         }
    //         else
    //         {
    //             cairo_set_source_rgb (cr, 1, 0.0, 0);
    //         }
    //         jintVec collision_point = jintLineGetPosition(&bluePointVelLine, e->bluePoint.ca.collFrame);
    //         cairo_arc (cr, collision_point.v[0], (600-25) - collision_point.v[1], 3, 0, 2 * M_PI);
    //         cairo_fill (cr);
    //     }
    // }
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
