#include "collisionDiagramActor.h" 

#include <SDL2/SDL.h>

#include <cairo.h>

#include "../../engine/engine.h"
#include "../../herdingSheepsEngine/herdingSheepsEngine.h"
#include <jTypes.h>

static void drawVelocityVector(
        cairo_t * cr, const collisionDiagramActor * cdActor,
        const jintVec * start, const jintVec * end);
static void drawActorCollisionDiagram(
        cairo_t * cr, const objectActor * oActor);
static void collisionDiagramDrawActorVelocity(
        collisionDiagramActor * this, cairo_t * cr,
        const objectActor * oa);
static void collisionDiagramDrawCollisionPoints(
        collisionDiagramActor * this, cairo_t * cr);

void drawCollisionDiagram(void * pixels, int pitch, void * ctx)
{
    collisionDiagramActor * c = ctx;
    herdingSheepsEngine * e = c->a.eng->owner;
    memset(pixels, 0, jintRectGetHeight(&c->d.rect) * pitch);

    cairo_surface_t *cairosurf = cairo_image_surface_create_for_data (
            pixels,
            CAIRO_FORMAT_ARGB32,
            jintRectGetWidth(&c->d.rect),
            jintRectGetHeight(&c->d.rect),
            pitch);

    cairo_t * cr = cairo_create (cairosurf);

	cairo_set_source_rgb (cr, 0, 0, 1);

    // transform coordinates so that (0,0) is bottom left of screen
    cairo_scale(cr, 1, -1);
    cairo_translate(cr, 0, -jintRectGetHeight(&c->d.rect));

    HS_GAME_STATE currentState;
    SBStateMachineGetCurrentState(e->mainStateMachine, &currentState);

    // draw actors
    const objectActorList * oActorLs;
	cairo_set_source_rgb (cr, 1, 0, 0);
    for (oActorLs = e->objectActorList; oActorLs; oActorLs = oActorLs->next)
    {
        drawActorCollisionDiagram(cr, oActorLs->val);
        collisionDiagramDrawActorVelocity(c, cr, oActorLs->val);
	    cairo_set_source_rgb (cr, 0, 0, 1);
    }

    // draw collision points
    collisionDiagramDrawCollisionPoints(c, cr);
}

static void drawCollisionPoint(cairo_t * cr, objectActor * actor, jint frame);

static void collisionDiagramDrawCollisionPoints(
        collisionDiagramActor * this, cairo_t * cr)
{
    herdingSheepsEngine * hEng = this->a.eng->owner;

    if (!hEng->objectActorList)
        return;

    objectActor * focussedActor = hEng->objectActorList->val;
    jintList * collFrameNode;
    for (collFrameNode = hEng->collFrameList; collFrameNode != NULL; collFrameNode = collFrameNode->next)
    {
        drawCollisionPoint(cr, focussedActor, *collFrameNode->val);
    }
}

static void drawCollisionPoint(cairo_t * cr, objectActor * actor, jint frame)
{
	cairo_set_source_rgb (cr, 1, 0.5, 0);
    switch(actor->type)
    {
        case OBJECT_ACTOR_TYPE_UNSET:
        {
            // do nothing
            break;
        }
        case OBJECT_ACTOR_TYPE_POINT:
        {
            jintVec r;
            pointActor * pa = actor->ptr.pa;
            pointActorGetPositionAtFrame(pa, frame, &r);
            cairo_arc (cr, r.v[0], r.v[1], 5, 0, 2 * M_PI);
            cairo_fill (cr);
            break;
        }
        case OBJECT_ACTOR_TYPE_V_LINE:
        case OBJECT_ACTOR_TYPE_H_LINE:
        {
            jint axis = 0;
            if (actor->type == OBJECT_ACTOR_TYPE_V_LINE)
            {
                axis = 1;
            }

            jintAxPlLine ln;
            lineActor * la = actor->ptr.la;
            lineActorGetLineAtFrame(la, frame, &ln);
            cairo_move_to(cr, ln.rStart.v[0], ln.rStart.v[1]);
            jint lt[2] = {0, 0};
            lt[axis] = ln.length;
            cairo_rel_line_to(cr, lt[0], lt[1]);
            cairo_stroke(cr);
            break;
        }
        case OBJECT_ACTOR_TYPE_RECT:
        {
            jintRect rect;
            rectActor * ra = actor->ptr.ra;
            rectActorGetRectAtFrame(ra, frame, &rect);
            cairo_rectangle(cr, rect.bl.v[0], rect.bl.v[1], 
                    jintRectGetWidth(&rect), jintRectGetHeight(&rect));
            cairo_stroke(cr);
            break;
        }
    }
}

static void drawVelocityVector(
        cairo_t * cr, const collisionDiagramActor * cdActor,
        const jintVec * start, const jintVec * end)
{
    const engine * eng = cdActor->a.eng;

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
        u = vel.v[0] ? (jintRectGetWidth(&cdActor->d.rect) - end->v[0]) 
            / vel.v[0] + 1: 0;
    }
    else
    {
        u = vel.v[0] ? -1 * end->v[0] / vel.v[0] + 1: 0;
    }

    if (vel.v[1] > 0)
    {
        v = vel.v[1] ? (jintRectGetHeight(&cdActor->d.rect) - end->v[1])
            / vel.v[1] + 1 : 0;
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
        int tickx = i * vel.v[0] / (jint)eng->fps + end->v[0];
        int ticky = i * vel.v[1] / (jint)eng->fps + end->v[1];

        if (tickx > jintRectGetWidth(&cdActor->d.rect)+5 || tickx < 0)
            break;

        if (ticky > jintRectGetHeight(&cdActor->d.rect)+5 || ticky < 0)
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
        int tickx = i * vel.v[0] / (jint)eng->fps + start->v[0];
        int ticky = i * vel.v[1] / (jint)eng->fps + start->v[1];

        cairo_arc (cr, tickx, ticky, 5, 0, 2 * M_PI);
        cairo_fill (cr);
    }
}

static void drawActorCollisionDiagram(
        cairo_t * cr, const objectActor * oActor)
{
    switch(oActor->type)
    {
        case OBJECT_ACTOR_TYPE_UNSET:
        {
            // do nothing
            break;
        }
        case OBJECT_ACTOR_TYPE_POINT:
        {
            jintVec r;
            pointActor * pa = oActor->ptr.pa;
            pointActorGetPosition(pa, &r);
            cairo_arc (cr, r.v[0], r.v[1], 30, 0, 2 * M_PI);
            cairo_fill (cr);
            break;
        }
        case OBJECT_ACTOR_TYPE_V_LINE:
        {
            jintAxPlLine ln;
            lineActor * la = oActor->ptr.la;
            lineActorGetLine(la, &ln);
            cairo_move_to(cr, ln.rStart.v[0], ln.rStart.v[1]);
            cairo_rel_line_to(cr, 0, ln.length);
            cairo_stroke(cr);
            break;
        }
        case OBJECT_ACTOR_TYPE_H_LINE:
        {
            jintAxPlLine ln;
            lineActor * la = oActor->ptr.la;
            lineActorGetLine(la, &ln);
            cairo_move_to(cr, ln.rStart.v[0], ln.rStart.v[1]);
            cairo_rel_line_to(cr, ln.length, 0);
            cairo_stroke(cr);
            break;
        }
        case OBJECT_ACTOR_TYPE_RECT:
        {
            jintRect rect;
            rectActor * ra = oActor->ptr.ra;
            rectActorGetRect(ra, &rect);
            cairo_rectangle(cr, rect.bl.v[0], rect.bl.v[1], 
                    jintRectGetWidth(&rect), jintRectGetHeight(&rect));
            cairo_stroke(cr);
            break;
        }
    }
}

static void collisionDiagramDrawActorVelocity(
        collisionDiagramActor * this, cairo_t * cr,
        const objectActor * oa)
{
    jintVec lStart, lEnd;
    switch(oa->type)
    {
        case OBJECT_ACTOR_TYPE_UNSET:
        {
            // do nothing
            break;
        }
        case OBJECT_ACTOR_TYPE_POINT:
        {
            pointActor * pa = oa->ptr.pa;
            pointActorGetPositionAtFrame(
                    pa, 0, &lStart);
            lEnd = jintVecAdd(lStart, pa->ca.vel.v);
            break;
        }
        case OBJECT_ACTOR_TYPE_V_LINE:
        case OBJECT_ACTOR_TYPE_H_LINE:
        {
            jintAxPlLine ln;
            lineActor * la = oa->ptr.la;
            lineActorGetLineAtFrame(
                    la, 0, &ln);
            int index = 0;
            if (oa->type == OBJECT_ACTOR_TYPE_V_LINE)
                index = 1; 
            lStart = ln.rStart;
            lStart.v[index] += ln.length/2;
            lEnd = jintVecAdd(lStart, la->ca.vel.v);
            break;
        }
        case OBJECT_ACTOR_TYPE_RECT:
        {
            rectActor * ra = oa->ptr.ra;
            jintRect rect;
            rectActorGetRectAtFrame(
                    ra, 0, &rect);
            lStart = rect.bl;
            lStart.v[0] += jintRectGetWidth(&rect)/2;
            lStart.v[1] += jintRectGetHeight(&rect)/2;
            lEnd = jintVecAdd(lStart, ra->ca.vel.v);
            break;
        }
    }
    drawVelocityVector(cr, this, &lStart, &lEnd);
}

void collisionDiagramRenderer(actor * a)
{
    collisionDiagramActor * c = (collisionDiagramActor *)a->owner;

    engineUpdateTexturesPixels(a->eng, c->d.textureId, drawCollisionDiagram, c);

    engineSpriteRender(a->eng, &c->s);
}

void initCollisionDiagram(engine * eng, collisionDiagramActor * c,
        const collisionDiagramParams * params)
{
    c->a.owner = c;
    c->a.renderHandler = collisionDiagramRenderer;
    c->a.logicHandler = NULL;
    juint collisionDiagramTexture = engineCreateTexture(
            eng, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 
            jintRectGetWidth(&params->window),
            jintRectGetHeight(&params->window));
    decalInit(&c->d, eng, collisionDiagramTexture, params->window);
    c->s.d = &c->d;
    c->s.rect = params->window;
    engineActorReg(eng, &c->a);
}
