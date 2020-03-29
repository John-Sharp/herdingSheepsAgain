#ifndef MAIN_ACTOR_H
#define MAIN_ACTOR_H

#include "../movingPointActor/movingPointActor.h"
#include "../lineActor/lineActor.h"

typedef union mainActorPtr
{
    movingPointActor * pt;
    lineActor * la;
} mainActorPtr;

typedef enum MAIN_ACTOR_TYPE
{
    MAIN_ACTOR_TYPE_UNSET,
    MAIN_ACTOR_TYPE_POINT,
    MAIN_ACTOR_TYPE_V_LINE
} MAIN_ACTOR_TYPE;

typedef struct mainActor
{
    MAIN_ACTOR_TYPE type;
    mainActorPtr ptr;
} mainActor;


#endif
