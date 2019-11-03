#include "wallActor.h" 

#include "../../herdingSheepsEngine/herdingSheepsEngine.h"

void initWallActor(herdingSheepsEngine * eng, wallActor * a, jintAxPlLine line);

#include "../../herdingSheepsEngine/listCode/wallActorList.inc"

void initWallActors(herdingSheepsEngine * eng)
{
    jintAxPlLine lines[] = {
        {
            .direction = AX_PL_DIR_Y,
            .rStart = {300, 10},
            .length = 150
        }
    };

    eng->walls = NULL;

    int i;
    for (i = 0; i < sizeof(lines)/sizeof(lines[0]); i++)
    {
        wallActor * wa = malloc(sizeof(*wa));
        if (!wa)
        {
            fprintf(stderr, "failed to allocate memory for wall actor");
            exit(1);
        }

        initWallActor(eng, wa, lines[i]);
    }
}

void initWallActor(herdingSheepsEngine * eng, wallActor * a, jintAxPlLine line)
{
    a->a.owner = a;
    a->a.renderHandler = NULL;
    a->a.logicHandler = NULL;

    collActor ca = {
        .type = line.direction == AX_PL_DIR_Y ? COLL_ACTOR_TYPE_V_LINE : COLL_ACTOR_TYPE_H_LINE,
        .shape = {
            .line = line 
        }
    };
    a->ca = ca;
    a->line = &a->ca.shape.line;
    eng->walls = wallActorListAdd(eng->walls, a);
}
