#include "wallActor.h" 

void initWallActor(engine * eng, wallActor * a)
{
    a->a.owner = a;
    a->a.renderHandler = NULL;
    a->a.logicHandler = NULL;
    a->pos.x = 300;
    a->pos.y = 10;
    a->length = 150;
}
