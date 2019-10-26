#include "wallActor.h" 

void initWallActor(engine * eng, wallActor * a)
{
    a->a.owner = a;
    a->a.renderHandler = NULL;
    a->a.logicHandler = NULL;

    jintVec rStart = {.x = 300, .y = 10};
    a-> line = createJintAxPlLine(
            AX_PL_DIR_Y, 
            rStart,
            150);
}
