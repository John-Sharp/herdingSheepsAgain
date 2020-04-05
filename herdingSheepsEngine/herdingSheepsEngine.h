#ifndef HERDING_SHEEPS_ENGINE_H
#define HERDING_SHEEPS_ENGINE_H

#include "../engine/engine.h"
#include <jTypes.h>
#include <collDetect.h>

#include "../actors/collisionDiagramActor/collisionDiagramActor.h"
#include "../actors/textBoxActor/textBoxActor.h"
#include "../actors/mainActor/mainActor.h"

#include "listHeaders/lineActorList.h"

#include <studiousBroccoli.h>

typedef struct herdingSheepsEngine {
    engine * engine;

    collisionDiagramActor collisionDiagram;
    textBoxActor frameRateBar;
    textBoxActor infoBar;

    struct mainActor mainActor;

    lineActorList * walls;

    jintRect mainWindow;

    SBStateMachine * mainStateMachine;
} herdingSheepsEngine;

herdingSheepsEngine * initHerdingSheepsEngine(herdingSheepsEngine * eng);
void herdingSheepsEngineSwitchMainObject(herdingSheepsEngine * eng, MAIN_ACTOR_TYPE type);
MAIN_ACTOR_TYPE herdingSheepsEngineGetMainObjectType(herdingSheepsEngine * eng);

void herdingSheepsEnginePushOtherObject(herdingSheepsEngine * this, MAIN_ACTOR_TYPE type);

typedef enum HS_GAME_STATE {
    HS_GAME_STATE_CHOOSE_MAIN_OBJECT,
    HS_GAME_STATE_MAIN_OBJECT_H_LINE,
    HS_GAME_STATE_MAIN_OBJECT_V_LINE,
    HS_GAME_STATE_MAIN_OBJECT_POINT,
    HS_GAME_STATE_MAIN_OBJECT_CHOOSE_VELOCITY,
    HS_GAME_STATE_MAIN_OBJECT_CHOOSE_DIMENSION,
    HS_GAME_STATE_CHOOSE_OTHER_OBJECT,
    HS_GAME_STATE_RUNNING
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
    KEYPRESS_STATE_P,
    KEYPRESS_STATE_V,
    KEYPRESS_STATE_H,
    KEYPRESS_STATE_ESC,
    KEYPRESS_STATE_SPACE
} KEYPRESS_STATE;

#endif
