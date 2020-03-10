#ifndef HERDING_SHEEPS_ENGINE_H
#define HERDING_SHEEPS_ENGINE_H

#include "../engine/engine.h"
#include <jTypes.h>
#include <collDetect.h>

#include "../actors/collisionDiagramActor/collisionDiagramActor.h"
#include "../actors/textBoxActor/textBoxActor.h"
#include "../actors/movingPointActor/movingPointActor.h"
#include "../actors/wallActor/wallActor.h"

#include "listHeaders/wallActorList.h"

#include <studiousBroccoli.h>

typedef struct herdingSheepsEngine {
    engine * engine;

    collisionDiagramActor collisionDiagram;
    textBoxActor frameRateBar;

    movingPointActor bluePoint;

    wallActorList * walls;

    jintRect mainWindow;

    SBStateMachine * mainStateMachine;
} herdingSheepsEngine;

herdingSheepsEngine * initHerdingSheepsEngine(herdingSheepsEngine * eng);

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
    KEYPRESS_STATE_ESC
} KEYPRESS_STATE;

#endif
