#include "wallActor.h" 

void initWallActor(engine * eng, wallActor * a)
{
    a->a.owner = a;
    a->a.renderHandler = NULL;
    a->a.logicHandler = NULL;
    a->pos[0] = 300;
    a->pos[1] = 10;
    a->length = 150;
}
