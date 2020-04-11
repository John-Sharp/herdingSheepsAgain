#include "objectActor.h"

void objectActorSetVelocity(
        objectActor * this, const jintVecScaled * v)
{
    this->objectActorSetVelocity(this, v);
}

void objectActorResetAppearence(
        objectActor * this)
{
    this->objectActorResetAppearence(this);
}

collActor * objectActorGetCollActor(
        const objectActor * this)
{
    return this->objectActorGetCollActor(this);
}
