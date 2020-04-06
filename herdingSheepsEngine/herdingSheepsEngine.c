#include "herdingSheepsEngine.h"
#include "HSStateMachine/HSStateMachine.h"
#include "../actors/textBoxActor/frameRateBar/frameRateBar.h"
#include "../actors/textBoxActor/infoBox/infoBox.h"
#include <assert.h>

#include "listCode/objectActorList.inc"

typedef enum STARTUP_CLICK_STATE {
    STARTUP_CLICK_STATE_POSITION_SELECT,
    STARTUP_CLICK_STATE_VELOCITY_SELECT,
} STARTUP_CLICK_STATE;

const juint KEYPRESS_REFRESH_MS = 250;

void herdingSheepsPreRender(engine * e)
{
    herdingSheepsEngine * hsEng = e->owner;
    HSStateMachineProcessInput(hsEng->mainStateMachine);

    // if (isStateActive(KEYPRESS_STATE_FRAME_PAUSE))
    // {
    //     engineIsPaused(e) ? engineUnpause(e) : enginePause(e);
    //     deactivateState(KEYPRESS_STATE_FRAME_PAUSE);
    // }
    // if (engineIsPaused(e))
    // {
    //     Uint32 time = SDL_GetTicks();
    //     static Uint32 lastTime = 0;
    //     if (isStateActive(KEYPRESS_STATE_FORWARD_FRAME))
    //     {
    //         if (isStateActive(KEYPRESS_STATE_FORWARD_FRAME_MULTIPLIER))
    //         {
    //             if (time - lastTime > KEYPRESS_REFRESH_MS)
    //             {
    //                 lastTime = time;
    //                 engineAdvanceFrames(e, 10);
    //             }
    //         }
    //         else
    //         {
    //             if (time - lastTime > KEYPRESS_REFRESH_MS)
    //             {
    //                 lastTime = time;
    //                 engineAdvanceFrames(e, 1);
    //             }
    //         }
    //     }
    //     else
    //     {
    //         lastTime = 0;
    //     }
    // }
}

STARTUP_CLICK_STATE startupClickState;
void startUpClicksCB(jint x, jint y, void * owner)
{
    // herdingSheepsEngine * eng = (herdingSheepsEngine *)owner;
    // if (startupClickState == STARTUP_CLICK_STATE_POSITION_SELECT)
    // {
    //     eng->bluePoint.ca.shape.point.v[0] = x;
    //     eng->bluePoint.ca.shape.point.v[1] = (600 - y);

    //     startupClickState = STARTUP_CLICK_STATE_VELOCITY_SELECT;
    // }
    // else if (startupClickState == STARTUP_CLICK_STATE_VELOCITY_SELECT)
    // {
    //     eng->bluePoint.ca.vel.v[0] = x - eng->bluePoint.ca.shape.point.v[0];
    //     eng->bluePoint.ca.vel.v[1] = (600 - y) - eng->bluePoint.ca.shape.point.v[1];
    //     eng->bluePoint.frameStart = eng->engine->currentFrame;

    //     // calculate next collision frame here
    //     wallActorList * wallNode = NULL;
    //     for (wallNode = eng->walls; wallNode != NULL; wallNode = wallNode->next)
    //     {
    //         wallActor * wall = wallNode->val;

    //         int frame = -1;
    //         if (calculateNextCollisionFrame(
    //                     &frame, &eng->bluePoint.ca.vel,
    //                     &eng->bluePoint.ca, &wall->ca) == COLL_FRAME_CALC_NO_COLLISION)
    //         {
    //             printf("no collision detected\n\n");
    //             frame = -1;
    //         }
    //         eng->bluePoint.ca.collFrame = frame;
    //         wall->ca.collFrame = frame;
    //     }
    // }
}

herdingSheepsEngine * initHerdingSheepsEngine(herdingSheepsEngine * eng)
{
    eng->engine = createEngine(800, 600, eng);

    enginePreRenderCallBackReg(eng->engine, herdingSheepsPreRender);

    // setup collisionDiagram
    {
    collisionDiagramParams params = {
        .window = {.bl = {.v = {0, 0}}, .tr = {.v = {800, 550}}}
    };
    initCollisionDiagram(eng->engine, &eng->collisionDiagram,
            &params);
    }

    // setup frameRateBar
    {
    textBoxParams params = {
        .window = {.bl = {.v = {0,575}}, .tr = {.v = {800, 600}}},
        .fg = {1,1,1},
        .bg = {1,0,0},
        .fontSize = 15,
        .hasRefreshedTextFn = hasRefreshedFrameRateBarText
    };
    initTextBox(eng->engine, &eng->frameRateBar,
            &params);
    }

    // setup infoBox
    {
    textBoxParams params = {
        .window = {.bl = {.v = {0,550}}, .tr = {.v = {800, 575}}},
        .fg = {1,1,1},
        .bg = {0,0,1},
        .fontSize = 15,
        .hasRefreshedTextFn = hasRefreshedInfoBoxText
    };
    initTextBox(eng->engine, &eng->infoBar,
            &params);
    }

    // setup main actor
    eng->mainActor.type = OBJECT_ACTOR_TYPE_UNSET;

    // setup other actor list
    eng->otherActorList = NULL;

    // mouseCallbackBinding mouseBinding = {
    //     .type = SDL_MOUSEBUTTONDOWN,
    //     .button = SDL_BUTTON_LEFT,
    //     .callback = startUpClicksCB,
    //     .owner = eng
    // };
    // addMouseCallback(&mouseBinding);

    // keyStateBinding ksb = {
    //     .k = SDLK_SPACE,
    //     .s = KEYPRESS_STATE_FRAME_PAUSE,
    //     .t = BINDING_ONE_TIME
    // };
    // addBinding(&ksb);

    // ksb.k = SDLK_RIGHT;
    // ksb.s = KEYPRESS_STATE_FORWARD_FRAME;
    // ksb.t = BINDING_CONTINUOUS;
    // addBinding(&ksb);

    // ksb.k = SDLK_LCTRL;
    // ksb.s = KEYPRESS_STATE_FORWARD_FRAME_MULTIPLIER;
    // ksb.t = BINDING_CONTINUOUS;
    // addBinding(&ksb);

    eng->mainStateMachine = createHSStateMachine(eng);

    return eng;
}

static objectActor herdingSheepsEngineCreateObjectActor(herdingSheepsEngine * this, OBJECT_ACTOR_TYPE type)
{
    switch (type)
    {
        case OBJECT_ACTOR_TYPE_POINT:
        {
            pointActor * pa = createPointActor(this->engine);
            assert(pa);
            return pa->oa;
        }
        case OBJECT_ACTOR_TYPE_H_LINE:
        {
            lineActorParams params = {
                .line = {
                    .direction = AX_PL_DIR_X,
                    .rStart = {.v = {10, 10}},
                    .length = 25
                }
            };

            lineActor * la = createLineActor(this->engine, &params);
            assert(la);
            return la->oa;
        }
        case OBJECT_ACTOR_TYPE_V_LINE:
        {
            lineActorParams params = {
                .line = {
                    .direction = AX_PL_DIR_Y,
                    .rStart = {.v = {10, 10}},
                    .length = 25
                }
            };

            lineActor *la = createLineActor(this->engine, &params);
            assert(la);
            return la->oa;
        }
        default:
        {
            objectActor ret = {.type=OBJECT_ACTOR_TYPE_UNSET};
            return  ret;
        }
    }
    objectActor ret = {.type=OBJECT_ACTOR_TYPE_UNSET};
    return  ret;
}

void herdingSheepsEngineSwitchMainObject(herdingSheepsEngine * eng, OBJECT_ACTOR_TYPE type)
{
    if (eng->mainActor.type == type)
    {
        if (eng->mainActor.type == OBJECT_ACTOR_TYPE_V_LINE ||
                eng->mainActor.type == OBJECT_ACTOR_TYPE_H_LINE)
        {
            eng->mainActor.ptr.la->ca.shape.line.length = 30;
        }
        return;
    }
    switch (eng->mainActor.type)
    {
        case OBJECT_ACTOR_TYPE_POINT:
        {
            pointActorDeinit(eng->mainActor.ptr.pa);
            free(eng->mainActor.ptr.pa);
            break;
        }
        case OBJECT_ACTOR_TYPE_V_LINE:
        case OBJECT_ACTOR_TYPE_H_LINE:
        {
            lineActorDeinit(eng->mainActor.ptr.la);
            free(eng->mainActor.ptr.la);
            break;
        }
        case OBJECT_ACTOR_TYPE_UNSET:
        {
            break;
        }
    }

    eng->mainActor = herdingSheepsEngineCreateObjectActor(eng, type);
}

OBJECT_ACTOR_TYPE herdingSheepsEngineGetMainObjectType(herdingSheepsEngine * eng)
{
    return eng->mainActor.type;
}

OBJECT_ACTOR_TYPE herdingSheepsEngineGetFocussedObjectType(
        herdingSheepsEngine * eng)
{
    if (!eng->otherActorList)
        return OBJECT_ACTOR_TYPE_UNSET;
    return eng->otherActorList->val->type;
}

void herdingSheepsEnginePushOtherObject(herdingSheepsEngine * this, OBJECT_ACTOR_TYPE type)
{
    objectActor * oa = malloc(sizeof(*oa));
    assert(oa);
    *oa = herdingSheepsEngineCreateObjectActor(this, type);
    this->otherActorList = objectActorListAdd(
            this->otherActorList,
            oa);
}
