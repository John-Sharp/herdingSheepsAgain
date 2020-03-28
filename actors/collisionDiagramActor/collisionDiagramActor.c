#include "collisionDiagramActor.h" 

#include <SDL2/SDL.h>

#include <cairo.h>

#include "../../engine/engine.h"
#include "../../herdingSheepsEngine/herdingSheepsEngine.h"
#include <jTypes.h>

static void drawVelocityVector(
        cairo_t * cr, const jintVec * start, const jintVec * end);

void drawCollisionDiagram(void * pixels, int pitch, void * ctx)
{
    herdingSheepsEngine * e = ((engine *)ctx)->owner;
    memset(pixels, 0, (600 - 50) * pitch);

    cairo_surface_t *cairosurf = cairo_image_surface_create_for_data (
            pixels,
            CAIRO_FORMAT_ARGB32,
            800,
            600 - 50,
            pitch);

    cairo_t * cr = cairo_create (cairosurf);

	cairo_set_source_rgb (cr, 0, 0, 1);

    // draw mainActor
    switch(e->mainActor.type)
    {
        case MAIN_ACTOR_TYPE_UNSET:
        {
            // do nothing
            break;
        }
        case MAIN_ACTOR_TYPE_POINT:
        {
            jintVec r;
            movingPointActor * mpa = e->mainActor.ptr.pt;
            movingPointActorGetPosition(mpa, &r.v[0], &r.v[1], e->engine->currentFrame);
            cairo_arc (cr, r.v[0], (600-50) - r.v[1], 30, 0, 2 * M_PI);
            cairo_fill (cr);
            break;
        }
        case MAIN_ACTOR_TYPE_WALL:
        {
            // TODO
            break;
        }
    }

    HS_GAME_STATE currentState;
    SBStateMachineGetCurrentState(e->mainStateMachine, &currentState);
    if (currentState == HS_GAME_STATE_MAIN_OBJECT_CHOOSE_VELOCITY)
    {
        jintVec lStart, lEnd;
        SDL_GetMouseState(&lEnd.v[0], &lEnd.v[1]);
        lEnd.v[1] = lEnd.v[1] - 50;
        switch(e->mainActor.type)
        {
            case MAIN_ACTOR_TYPE_UNSET:
            {
                // do nothing
                break;
            }
            case MAIN_ACTOR_TYPE_POINT:
            {
                movingPointActor * mpa = e->mainActor.ptr.pt;
                movingPointActorGetPosition(
                        mpa, &lStart.v[0], &lStart.v[1], e->engine->currentFrame);
                lStart.v[1] = (600-50) - lStart.v[1];

                break;
            }
            case MAIN_ACTOR_TYPE_WALL:
            {
                // TODO
                break;
            }
        }
        drawVelocityVector(cr, &lStart, &lEnd);
    }

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

static void drawVelocityVector(
        cairo_t * cr, const jintVec * start, const jintVec * end)
{
    // draw extended velocity vector
    jintVec vel;
    vel.v[0] = end->v[0] - start->v[0];
    vel.v[1] = end->v[1] - start->v[1];

    if (vel.v[0] == 0 && vel.v[1] == 0)
    {
        return;
    }

    cairo_set_source_rgb (cr, 1, 1, 0);
    cairo_move_to(cr, end->v[0], end->v[1]);
    jint u, v;

    if (vel.v[0] > 0)
    {
        u = vel.v[0] ? (800 - end->v[0]) / vel.v[0] + 1: 0;
    }
    else
    {
        u = vel.v[0] ? -1 * end->v[0] / vel.v[0] + 1: 0;
    }

    if (vel.v[1] > 0)
    {
        v = vel.v[1] ? ((600 - 50) - end->v[1]) / vel.v[1] + 1 : 0;
    }
    else
    {
        v = vel.v[1] ? -1 * end->v[1] / vel.v[1] + 1 : 0;
    }
    u = u > v ? u : v;

    cairo_rel_line_to(cr, u * vel.v[0], u * vel.v[1]);
    cairo_stroke(cr);

    // draw extended tick marks
    int i;
    for (i = 10; ; i+=10)
    {
        // TODO replace 80 with engine fps
        int tickx = i * vel.v[0] / 80 + end->v[0];
        int ticky = i * vel.v[1] / 80 + end->v[1];

        if (tickx > 800+5 || tickx < 0)
            break;

        if (ticky > 600-50+5 || ticky < 0)
            break;

        cairo_arc (cr, tickx, ticky, 5, 0, 2 * M_PI);
        cairo_fill (cr);
    }

    // draw velocity vector
    cairo_set_source_rgb (cr, 0, 1, 0);
    cairo_move_to(cr, start->v[0], start->v[1]);
    cairo_line_to(cr, end->v[0], end->v[1]);
    cairo_stroke(cr);

    // draw tickmarks
    for (i = 10; i <= 80; i+=10)
    {
        int tickx = i * vel.v[0] / 80 + start->v[0];
        int ticky = i * vel.v[1] / 80 + start->v[1];

        cairo_arc (cr, tickx, ticky, 5, 0, 2 * M_PI);
        cairo_fill (cr);
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
            eng, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 800, 600 - 50);
    decalInit(&c->d, eng, collisionDiagramTexture, createJintRect(0, 0, 800, 600 - 50));
    c->s.d = &c->d;
    c->s.rect = createJintRect(0, 0, 800, 600 -50);
    engineActorReg(eng, &c->a);
}
