#include "textBoxActor.h" 

#include <SDL2/SDL.h>

#include <cairo.h>

static cairo_t * cr_global;
void textReceiverImpl(const char * text)
{
    cairo_show_text(cr_global, text);
}
static textProvider getTextBoxText;

void drawTextBox(void * pixels, int pitch, void * ctx)
{
    textBoxActor * t = (textBoxActor *)ctx;

    jint textBoxWidth = t->window.tr.v[0] - t->window.bl.v[0];
    jint textBoxHeight = t->window.tr.v[1] - t->window.bl.v[1];

    memset(pixels, 0, textBoxHeight * pitch);

    cairo_surface_t *cairosurf = cairo_image_surface_create_for_data (
            pixels,
            CAIRO_FORMAT_ARGB32,
            textBoxWidth,
            textBoxHeight,
            pitch);

    cairo_t * cr = cairo_create (cairosurf);

	cairo_set_source_rgb (cr, t->bgColor[0], t->bgColor[1], t->bgColor[2]);
	cairo_paint (cr);

	cairo_set_source_rgb (cr, t->fgColor[0], t->fgColor[1], t->fgColor[2]);
    cairo_move_to(cr, 20, 18);
    cairo_set_font_size(cr, t->fontSize);

    cr_global = cr;
    getTextBoxText(t, textReceiverImpl);
}

void textBoxRenderer(actor * a)
{
    textBoxActor * t = (textBoxActor *)a->owner;

    if ((getTextBoxText = t->hasRefreshedText(t)))
    {
        engineUpdateTexturesPixels(a->eng, t->d.textureId, drawTextBox, t);
    }

    engineSpriteRender(a->eng, &t->s);
}

void initTextBox(
        engine * eng,
        textBoxActor * t,
        const textBoxParams * params
        )
{
    t->window = params->window;
    jint textBoxWidth = t->window.tr.v[0] - t->window.bl.v[0];
    jint textBoxHeight = t->window.tr.v[1] - t->window.bl.v[1];
    t->a.owner = t;
    t->a.renderHandler = textBoxRenderer;
    t->a.logicHandler = NULL;
    juint textBoxTexture = engineCreateTexture(
            eng, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
            textBoxWidth, textBoxHeight);
    decalInit(&t->d, eng, textBoxTexture,
            createJintRect(0,0,textBoxWidth, textBoxHeight));
    t->s.d = &t->d;
    t->s.rect = t->window;

    memcpy(t->bgColor, params->bg, sizeof(t->bgColor));
    memcpy(t->fgColor, params->fg, sizeof(t->fgColor));
    t->fontSize = params->fontSize;
    t->hasRefreshedText = params->hasRefreshedTextFn;
    t->prevUpdate = 0;

    engineActorReg(eng, &t->a);
}
