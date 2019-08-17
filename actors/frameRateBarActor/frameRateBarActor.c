#include "frameRateBarActor.h" 

#include <SDL2/SDL.h>

#include <cairo.h>

void drawFrameRateBar(void * pixels, int pitch, void * ctx)
{
    frameRateBarActor * f = (frameRateBarActor *)ctx;
    memset(pixels, 0, 25 * pitch);

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
    sprintf(frameRateInfoBuffer, "render FPS: %u, logic FPS: %u, current logic frame: %u",
            renderFPS, logicFPS, f->a.eng->currentFrame);
    cairo_show_text(cr, frameRateInfoBuffer);
}

void frameRateBarRenderer(actor * a)
{
    frameRateBarActor * f = (frameRateBarActor *)a->owner;
    static int prevUpdate = 0;

    int update = SDL_GetTicks() / 250;
    if (update > prevUpdate)
    {
        prevUpdate = update;
        engineUpdateTexturesPixels(a->eng, f->d.textureId, drawFrameRateBar, f);
    }

    engineSpriteRender(a->eng, &f->s);
}

void initFrameRateBar(engine * eng, frameRateBarActor * f)
{
    f->window = createJintRect(0,600-25, 800, 600);
    jint frameRateBarWidth = f->window.tr[0] - f->window.bl[0];
    jint frameRateBarHeight = f->window.tr[1] - f->window.bl[1];
    f->a.owner = f;
    f->a.renderHandler = frameRateBarRenderer;
    f->a.logicHandler = NULL;
    juint frameRateBarTexture = engineCreateTexture(
            eng, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
            frameRateBarWidth, frameRateBarHeight);
    decalInit(&f->d, eng, frameRateBarTexture,
            createJintRect(0,0,frameRateBarWidth, frameRateBarHeight));
    f->s.d = &f->d;
    f->s.rect = f->window;
    engineActorReg(eng, &f->a);
}
