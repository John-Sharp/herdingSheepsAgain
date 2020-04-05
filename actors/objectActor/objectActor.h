#ifndef OBJECT_ACTOR_H
#define OBJECT_ACTOR_H

typedef struct pointActor pointActor;
typedef struct lineActor lineActor;

typedef union objectActorPtr
{
    pointActor * pa;
    lineActor * la;
} objectActorPtr;

typedef enum OBJECT_ACTOR_TYPE
{
    OBJECT_ACTOR_TYPE_UNSET,
    OBJECT_ACTOR_TYPE_POINT,
    OBJECT_ACTOR_TYPE_V_LINE,
    OBJECT_ACTOR_TYPE_H_LINE
} OBJECT_ACTOR_TYPE;

typedef struct objectActor
{
    OBJECT_ACTOR_TYPE type;
    objectActorPtr ptr;
} objectActor;

#endif
