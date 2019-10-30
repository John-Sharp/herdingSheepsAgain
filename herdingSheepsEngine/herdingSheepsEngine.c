#include "herdingSheepsEngine.h"
#include "../collDetect/collDetect.h"

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
        eng->bluePoint.vLine.rStart.x = x;
        eng->bluePoint.vLine.rStart.y = (600 - y);

        startupClickState = STARTUP_CLICK_STATE_VELOCITY_SELECT;
    }
    else if (startupClickState == STARTUP_CLICK_STATE_VELOCITY_SELECT)
    {
        eng->bluePoint.vLine.sTarg.x = x - eng->bluePoint.vLine.rStart.x;
        eng->bluePoint.vLine.sTarg.y = (600 - y) - eng->bluePoint.vLine.rStart.y;
        eng->bluePoint.frameStart = eng->engine->currentFrame;

        // calculate next collision frame here
        jint frame;
        collActor ca1 = {
            .type = COLL_ACTOR_TYPE_POINT,
            .shape = {
                .point = eng->bluePoint.vLine
            }
        };
        collActor ca2 = {
            .type = COLL_ACTOR_TYPE_V_LINE,
            .shape = {
                .line = {
                    .direction = AX_PL_DIR_Y,
                    .rStart = {
                        .x = 300,
                        .y = 10
                    },
                    .length = 150
                }
            }
        };

        calculateNextCollisionFrame(
                &frame, &ca1, &ca2);
        printf("collision frame is %d\n", frame);
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
