#ifndef OBJECT_ACTOR_H
#define OBJECT_ACTOR_H

#include <jTypes.h>

typedef struct pointActor pointActor;
typedef struct lineActor lineActor;
typedef struct rectActor rectActor;
typedef struct collActor collActor;

typedef union objectActorPtr
{
    pointActor * pa;
    lineActor * la;
    rectActor * ra;
} objectActorPtr;

typedef enum OBJECT_ACTOR_TYPE
{
    OBJECT_ACTOR_TYPE_UNSET,
    OBJECT_ACTOR_TYPE_POINT,
    OBJECT_ACTOR_TYPE_V_LINE,
    OBJECT_ACTOR_TYPE_H_LINE,
    OBJECT_ACTOR_TYPE_RECT
} OBJECT_ACTOR_TYPE;

typedef struct objectActor objectActor;
typedef void (*objectActorSetVelocityFn)(
        objectActor * this, const jintVecScaled * v);
typedef collActor * (*objectActorGetCollActorFn)(
        const objectActor * this);
typedef void (*objectActorResetAppearenceFn)(
        objectActor * this);

typedef struct objectActor
{
    OBJECT_ACTOR_TYPE type;
    objectActorPtr ptr;

    objectActorSetVelocityFn objectActorSetVelocity;
    objectActorResetAppearenceFn objectActorResetAppearence;
    objectActorGetCollActorFn objectActorGetCollActor;
} objectActor;

void objectActorSetVelocity(
        objectActor * this, const jintVecScaled * v);
void objectActorResetAppearence(
        objectActor * this);

collActor * objectActorGetCollActor(
        const objectActor * this);
#endif
