#ifndef HERDING_SHEEPS_ENGINE_H
#define HERDING_SHEEPS_ENGINE_H

#include "../engine/engine.h"
#include <jTypes.h>
#include <collDetect.h>

#include "../actors/collisionDiagramActor/collisionDiagramActor.h"
#include "../actors/textBoxActor/textBoxActor.h"
#include "../actors/pointActor/pointActor.h"
#include "../actors/lineActor/lineActor.h"
#include "../actors/rectActor/rectActor.h"

#include "listHeaders/objectActorList.h"
#include "listHeaders/jintList.h"

#include <studiousBroccoli.h>

enum 
{
    HS_COLLISION_CATEGORY = 1
};

typedef struct herdingSheepsEngine {
    engine * engine;
    collEngine * collEngine;

    collisionDiagramActor collisionDiagram;
    textBoxActor frameRateBar;
    textBoxActor infoBar;

    objectActorList * objectActorList;
    jintList * collFrameList;

    jintRect mainWindow;

    SBStateMachine * mainStateMachine;
} herdingSheepsEngine;

herdingSheepsEngine * initHerdingSheepsEngine(herdingSheepsEngine * eng);
void herdingSheepsEngineCalculateFocussedObjectCollisionPoints(herdingSheepsEngine * this);

OBJECT_ACTOR_TYPE herdingSheepsEngineGetFocussedObjectType(
        herdingSheepsEngine * eng);

void herdingSheepsEnginePushObject(herdingSheepsEngine * this, OBJECT_ACTOR_TYPE type);
bool herdingSheepsEnginePopAndReleaseObject(
        herdingSheepsEngine * this);
void herdingSheepsEngineSwitchFocus(herdingSheepsEngine * this);

typedef enum HS_GAME_STATE {
    HS_GAME_STATE_ERROR,
    HS_GAME_STATE_CHOOSE_OBJECT,
    HS_GAME_STATE_OBJECT_CHOOSE_DIMENSION,
    HS_GAME_STATE_OBJECT_BEING_POSITIONED,
    HS_GAME_STATE_OBJECT_CHOOSE_VELOCITY,
    HS_GAME_STATE_RUNNING,
    HS_GAME_STATE_PAUSED
} HS_GAME_STATE;

// TODO this doesn't need to be here, can be moved once inputProcessor has
// getFreeState implemented
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
    KEYPRESS_STATE_F,
    KEYPRESS_STATE_P,
    KEYPRESS_STATE_V,
    KEYPRESS_STATE_H,
    KEYPRESS_STATE_R,
    KEYPRESS_STATE_ESC,
    KEYPRESS_STATE_SPACE,
    KEYPRESS_STATE_RIGHT,
    KEYPRESS_STATE_CTRL
} KEYPRESS_STATE;

#endif
