#include "herdingSheepsEngine.h"
#include "HSStateMachine/HSStateMachine.h"
#include "../actors/textBoxActor/frameRateBar/frameRateBar.h"
#include "../actors/textBoxActor/infoBox/infoBox.h"
#include <assert.h>

#include "listCode/objectActorList.inc"
#include "listCode/jintList.inc"

typedef enum STARTUP_CLICK_STATE {
    STARTUP_CLICK_STATE_POSITION_SELECT,
    STARTUP_CLICK_STATE_VELOCITY_SELECT,
} STARTUP_CLICK_STATE;

const juint KEYPRESS_REFRESH_MS = 250;

void herdingSheepsPreRender(engine * e)
{
    herdingSheepsEngine * hsEng = e->owner;
    HSStateMachineProcessInput(hsEng->mainStateMachine);
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

    // setup coll frame list
    eng->collFrameList = NULL;

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

void herdingSheepsEngineCalculateMainObjectCollisionPoints(herdingSheepsEngine * this)
{
    collActor * mainCollActor = objectActorGetCollActor(&this->mainActor);
    objectActorList * listNode;
    for (listNode = this->otherActorList; listNode != NULL; listNode = listNode->next)
    {
        collActor * otherCollActor = objectActorGetCollActor(listNode->val);
        jint collFrame;
        if (calculateNextCollisionFrame(
                    &collFrame,
                    &mainCollActor->vel,
                    mainCollActor, otherCollActor) == COLL_FRAME_CALC_OK)
        {
            jint * collFrameStorage = malloc(sizeof(*collFrameStorage));
            *collFrameStorage = collFrame;
            this->collFrameList = jintListAdd(
                this->collFrameList,
                collFrameStorage);
        }
    }
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

bool herdingSheepsEnginePopAndReleaseOtherObject(
        herdingSheepsEngine * this)
{
    if (!this->otherActorList)
        return false;

    objectActor * popped = this->otherActorList->val;
    this->otherActorList = objectActorListRm(
            this->otherActorList, popped, objectActorCmp, NULL);

    switch (popped->type)
    {
        case OBJECT_ACTOR_TYPE_POINT:
        {
            pointActorDeinit(popped->ptr.pa);
            free(popped->ptr.pa);
            break;
        }
        case OBJECT_ACTOR_TYPE_V_LINE:
        case OBJECT_ACTOR_TYPE_H_LINE:
        {
            lineActorDeinit(popped->ptr.la);
            free(popped->ptr.la);
            break;
        }
        case OBJECT_ACTOR_TYPE_UNSET:
        {
            break;
        }
    }
    free(popped);
    return true;
}
