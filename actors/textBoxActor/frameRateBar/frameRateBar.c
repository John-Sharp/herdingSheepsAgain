#include "frameRateBar.h"

void getFrameRateBarText(textBoxActor * t, textReceiver tr)
{
    uint32_t renderFPS, logicFPS;
    engineGetFrameRate(t->a.eng, &logicFPS, &renderFPS);
    char frameRateInfoBuffer[256];
    sprintf(frameRateInfoBuffer, "render FPS: %u, logic FPS: %u, current logic frame: %u",
            renderFPS, logicFPS, t->a.eng->currentFrame);
    tr(frameRateInfoBuffer);
    return;
}

textProvider hasRefreshedFrameRateBarText(textBoxActor * t)
{
    int update_rate_ms = 250;

    int update = SDL_GetTicks() / update_rate_ms;

    if (update > t->prevUpdate)
    {
        t->prevUpdate = update;
        return getFrameRateBarText;
    }
    return NULL;
}

