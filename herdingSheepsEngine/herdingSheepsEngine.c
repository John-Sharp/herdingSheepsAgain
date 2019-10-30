#include "herdingSheepsEngine.h"

typedef enum STARTUP_CLICK_STATE {
    STARTUP_CLICK_STATE_POSITION_SELECT,
    STARTUP_CLICK_STATE_VELOCITY_SELECT,
} STARTUP_CLICK_STATE;

typedef enum KEYPRESS_STATE {
    KEYPRESS_STATE_QUIT,
    KEYPRESS_STATE_FRAME_PAUSE,
    KEYPRESS_STATE_FORWARD_FRAME,
    KEYPRESS_STATE_FORWARD_FRAME_MULTIPLIER, // used for keeping track of
                                             // whether ctrl is pressed,
                                             // since this should make us
                                             // move forward 10 frames
                                             // rather than one if
                                             // KEYPRESS_STATE_FORWARD_FRAME_MULTIPLIER
                                             // is also active
} KEYPRESS_STATE;
const juint KEYPRESS_REFRESH_MS = 250;

void herdingSheepsPreRender(engine * e)
{
    if (isStateActive(KEYPRESS_STATE_FRAME_PAUSE))
    {
        engineIsPaused(e) ? engineUnpause(e) : enginePause(e);
        deactivateState(KEYPRESS_STATE_FRAME_PAUSE);
    }
    if (engineIsPaused(e))
    {
        Uint32 time = SDL_GetTicks();
        static Uint32 lastTime = 0;
        if (isStateActive(KEYPRESS_STATE_FORWARD_FRAME))
        {
            if (isStateActive(KEYPRESS_STATE_FORWARD_FRAME_MULTIPLIER))
            {
                if (time - lastTime > KEYPRESS_REFRESH_MS)
                {
                    lastTime = time;
                    engineAdvanceFrames(e, 10);
                }
            }
            else
            {
                if (time - lastTime > KEYPRESS_REFRESH_MS)
                {
                    lastTime = time;
                    engineAdvanceFrames(e, 1);
                }
            }
        }
        else
        {
            lastTime = 0;
        }
    }
}

STARTUP_CLICK_STATE startupClickState;
void startUpClicksCB(jint x, jint y, void * owner)
{
    herdingSheepsEngine * eng = (herdingSheepsEngine *)owner;
    if (startupClickState == STARTUP_CLICK_STATE_POSITION_SELECT)
    {
        eng->bluePoint.ca.shape.point.rStart.x = x;
        eng->bluePoint.ca.shape.point.rStart.y = (600 - y);

        startupClickState = STARTUP_CLICK_STATE_VELOCITY_SELECT;
    }
    else if (startupClickState == STARTUP_CLICK_STATE_VELOCITY_SELECT)
    {
        eng->bluePoint.ca.shape.point.sTarg.x = x - eng->bluePoint.ca.shape.point.rStart.x;
        eng->bluePoint.ca.shape.point.sTarg.y = (600 - y) - eng->bluePoint.ca.shape.point.rStart.y;
        eng->bluePoint.frameStart = eng->engine->currentFrame;

        // calculate next collision frame here
        int frame;
        calculateNextCollisionFrame(
                &frame, &eng->bluePoint.ca, &eng->wall.ca);
        eng->bluePoint.ca.collFrame = frame;
        eng->wall.ca.collFrame = frame;
    }
}

herdingSheepsEngine * initHerdingSheepsEngine(herdingSheepsEngine * eng)
{
    eng->engine = createEngine(800, 600, eng);

    enginePreRenderCallBackReg(eng->engine, herdingSheepsPreRender);

    // setup collisionDiagram
    initCollisionDiagram(eng->engine, &eng->collisionDiagram);

    // setup frameRateBar
    initFrameRateBar(eng->engine, &eng->frameRateBar);

    // setup bluePoint
    initMovingPointActor(eng->engine, &eng->bluePoint);

    // setup wallActors
    initWallActor(eng->engine, &eng->wall);

    mouseCallbackBinding mouseBinding = {
        .type = SDL_MOUSEBUTTONDOWN,
        .button = SDL_BUTTON_LEFT,
        .callback = startUpClicksCB,
        .owner = eng
    };
    addMouseCallback(&mouseBinding);

    keyStateBinding ksb = {
        .k = SDLK_SPACE,
        .s = KEYPRESS_STATE_FRAME_PAUSE,
        .t = BINDING_ONE_TIME
    };
    addBinding(&ksb);

    ksb.k = SDLK_RIGHT;
    ksb.s = KEYPRESS_STATE_FORWARD_FRAME;
    ksb.t = BINDING_CONTINUOUS;
    addBinding(&ksb);

    ksb.k = SDLK_LCTRL;
    ksb.s = KEYPRESS_STATE_FORWARD_FRAME_MULTIPLIER;
    ksb.t = BINDING_CONTINUOUS;
    addBinding(&ksb);

    return eng;
}
