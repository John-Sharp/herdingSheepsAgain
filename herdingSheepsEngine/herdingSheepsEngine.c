#include "herdingSheepsEngine.h"
#include "HSStateMachine/HSStateMachine.h"

typedef enum STARTUP_CLICK_STATE {
    STARTUP_CLICK_STATE_POSITION_SELECT,
    STARTUP_CLICK_STATE_VELOCITY_SELECT,
} STARTUP_CLICK_STATE;

const juint KEYPRESS_REFRESH_MS = 250;

void herdingSheepsPreRender(engine * e)
{
    herdingSheepsEngine * hsEng = e->owner;
    HSStateMachineProcessInput(hsEng->mainStateMachine);

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
        eng->bluePoint.ca.shape.point.v[0] = x;
        eng->bluePoint.ca.shape.point.v[1] = (600 - y);

        startupClickState = STARTUP_CLICK_STATE_VELOCITY_SELECT;
    }
    else if (startupClickState == STARTUP_CLICK_STATE_VELOCITY_SELECT)
    {
        eng->bluePoint.ca.vel.v[0] = x - eng->bluePoint.ca.shape.point.v[0];
        eng->bluePoint.ca.vel.v[1] = (600 - y) - eng->bluePoint.ca.shape.point.v[1];
        eng->bluePoint.frameStart = eng->engine->currentFrame;

        // calculate next collision frame here
        wallActorList * wallNode = NULL;
        for (wallNode = eng->walls; wallNode != NULL; wallNode = wallNode->next)
        {
            wallActor * wall = wallNode->val;

            int frame = -1;
            if (calculateNextCollisionFrame(
                        &frame, &eng->bluePoint.ca.vel,
                        &eng->bluePoint.ca, &wall->ca) == COLL_FRAME_CALC_NO_COLLISION)
            {
                printf("no collision detected\n\n");
                frame = -1;
            }
            eng->bluePoint.ca.collFrame = frame;
            wall->ca.collFrame = frame;
        }
    }
}

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

herdingSheepsEngine * initHerdingSheepsEngine(herdingSheepsEngine * eng)
{
    eng->engine = createEngine(800, 600, eng);

    enginePreRenderCallBackReg(eng->engine, herdingSheepsPreRender);

    // setup collisionDiagram
    initCollisionDiagram(eng->engine, &eng->collisionDiagram);

    // setup frameRateBar
    textBoxParams params = {
        .window = {.bl = {.v = {0,575}}, .tr = {.v = {800, 600}}},
        .fg = {1,1,1},
        .bg = {1,0,0},
        .fontSize = 15,
        .textProvider = getFrameRateBarText
    };
    initTextBox(eng->engine, &eng->frameRateBar,
            &params);

    // setup bluePoint
    initMovingPointActor(eng->engine, &eng->bluePoint);

    // setup wallActors
    initWallActors(eng);

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

    eng->mainStateMachine = createHSStateMachine(eng);

    return eng;
}
