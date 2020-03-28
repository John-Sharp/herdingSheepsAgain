#ifndef COLLISION_DIAGRAM_H
#define COLLISION_DIAGRAM_H

#include "../../engine/engine.h"
#include <jTypes.h>

typedef struct collisionDiagramParams {
    jintRect window;
} collisionDiagramParams;

typedef struct collisionDiagramActor {
    actor a;
    sprite s;
    decal d;
} collisionDiagramActor;

void initCollisionDiagram(engine * eng, collisionDiagramActor * f,
        const collisionDiagramParams * params);

#endif
