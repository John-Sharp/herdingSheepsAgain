#include "herdingSheepsEngine.h"
#include "HSStateMachine/HSStateMachine.h"
#include "../actors/textBoxActor/frameRateBar/frameRateBar.h"
#include "../actors/textBoxActor/infoBox/infoBox.h"
#include <assert.h>

#include "listCode/objectActorList.inc"
#include "listCode/jintList.inc"

#include <collisionFrameCalculate.h>

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

void herdingSheepsPreLogic(engine * e)
{
    herdingSheepsEngine * this = e->owner;

    HS_GAME_STATE currentState;
    SBStateMachineGetCurrentState(
        this->mainStateMachine, &currentState);
    if (currentState == HS_GAME_STATE_RUNNING)
    {
        collEngineProcessFrame(this->collEngine);
    }
}

void herdingSheepsCollHandler(
        collActor * ca1, collActor * ca2, const jintVec * norm)
{
    printf("I have been called, exiting (ca1 type: %u norm : (%d, %d))\n\n",
            ca1->type, norm->v[0], norm->v[1]);
    exit(0);
    // TODO set velocity to reverse here
}

herdingSheepsEngine * initHerdingSheepsEngine(herdingSheepsEngine * eng)
{
    eng->engine = createEngine(800, 600, eng);
    eng->collEngine = createCollEngine();
    collEngineUpsertCollGroup(eng->collEngine,
            HS_COLLISION_CATEGORY, HS_COLLISION_CATEGORY,
            herdingSheepsCollHandler);

    enginePreRenderCallBackReg(eng->engine, herdingSheepsPreRender);
    enginePreLogicCallBackReg(eng->engine, herdingSheepsPreLogic);

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

    // setup object actor list
    eng->objectActorList = NULL;

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
        case OBJECT_ACTOR_TYPE_RECT:
        {
            rectActorParams params = {
                .rect = {
                    .bl = {{10,10}},
                    .tr = {{35,35}}
                }
            };

            rectActor *ra = createRectActor(this->engine, &params);
            assert(ra);
            return ra->oa;
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

void herdingSheepsEngineCalculateFocussedObjectCollisionPoints(herdingSheepsEngine * this)
{
    if (!this->objectActorList)
        return;


    jintList * l;
    while ((l = this->collFrameList))
    {
        this->collFrameList = l->next;
        free(l->val);
        free(l);
    }

    collActor * focussedCollActor = objectActorGetCollActor(this->objectActorList->val);
    objectActorList * listNode;
    for (listNode = this->objectActorList->next; listNode != NULL; listNode = listNode->next)
    {
        collActor * otherCollActor = objectActorGetCollActor(listNode->val);
        jint collFrame;
        jintVec norm;
        if (calculateNextCollisionFrame(
                    &collFrame,
                    &norm,
                    focussedCollActor, otherCollActor) == COLL_FRAME_CALC_OK)
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
    if (!eng->objectActorList)
        return OBJECT_ACTOR_TYPE_UNSET;
    return eng->objectActorList->val->type;
}

void herdingSheepsEnginePushObject(herdingSheepsEngine * this, OBJECT_ACTOR_TYPE type)
{
    objectActor * oa = malloc(sizeof(*oa));
    assert(oa);
    *oa = herdingSheepsEngineCreateObjectActor(this, type);
    this->objectActorList = objectActorListAdd(
            this->objectActorList,
            oa);
}

bool herdingSheepsEnginePopAndReleaseObject(
        herdingSheepsEngine * this)
{
    if (!this->objectActorList)
        return false;

    objectActor * popped = this->objectActorList->val;
    this->objectActorList = objectActorListRm(
            this->objectActorList, popped, objectActorCmp, NULL);

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
        case OBJECT_ACTOR_TYPE_RECT:
        case OBJECT_ACTOR_TYPE_UNSET:
        {
            break;
        }
    }
    free(popped);
    return true;
}

void herdingSheepsEngineSwitchFocus(herdingSheepsEngine * this)
{
    if (!this->objectActorList)
        return;

    objectActorList * oldHead = this->objectActorList;
    this->objectActorList = this->objectActorList->next;
    objectActorList * node;
    for (node = this->objectActorList; node->next != NULL; node = node->next)
    {}
    node->next = oldHead;
    oldHead->next = NULL;
    herdingSheepsEngineCalculateFocussedObjectCollisionPoints(this);
}
