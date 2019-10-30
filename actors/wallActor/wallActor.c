#include "wallActor.h" 

void initWallActor(engine * eng, wallActor * a)
{
    a->a.owner = a;
    a->a.renderHandler = NULL;
    a->a.logicHandler = NULL;

    collActor ca = {
        .type = COLL_ACTOR_TYPE_V_LINE,
        .shape = {
            .line = {
                .direction = AX_PL_DIR_Y,
                .rStart = {300, 10},
                .length = 150
            }
        }
    };
    a->ca = ca;
    a->line = &a->ca.shape.line;
}
