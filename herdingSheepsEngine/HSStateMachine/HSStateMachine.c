#include "HSStateMachine.h"
#include "../actors/textBoxActor/infoBox/infoBox.h"

typedef enum HS_GAME_STATE_TOKEN {
    HS_GAME_STATE_TOKEN_P,
    HS_GAME_STATE_TOKEN_V,
    HS_GAME_STATE_TOKEN_H,
    HS_GAME_STATE_TOKEN_R,
    HS_GAME_STATE_TOKEN_L_CLICK,
    HS_GAME_STATE_TOKEN_ESC,
    HS_GAME_STATE_TOKEN_SPACE,
    HS_GAME_STATE_TOKEN_RIGHT,
    HS_GAME_STATE_TOKEN_RIGHT_MODIFIED
} HS_GAME_STATE_TOKEN;

static juint pointObjectChosen(SBStateMachine * stateMachine, juint token)
{
    setTextToAddObjectPoint();
    herdingSheepsEnginePushObject(stateMachine->context, OBJECT_ACTOR_TYPE_POINT);
    return HS_GAME_STATE_OBJECT_BEING_POSITIONED;
}

static juint VLineObjectChosen(SBStateMachine * stateMachine, juint token)
{
    setTextToAddObjectVLine();
    herdingSheepsEnginePushObject(stateMachine->context, OBJECT_ACTOR_TYPE_V_LINE);
    return HS_GAME_STATE_OBJECT_BEING_POSITIONED;
}

static juint HLineObjectChosen(SBStateMachine * stateMachine, juint token)
{
    setTextToAddObjectHLine();
    herdingSheepsEnginePushObject(stateMachine->context, OBJECT_ACTOR_TYPE_H_LINE);
    return HS_GAME_STATE_OBJECT_BEING_POSITIONED;
}

static juint rectObjectChosen(SBStateMachine * stateMachine, juint token)
{
    setTextToAddObjectRect();
    herdingSheepsEnginePushObject(stateMachine->context, OBJECT_ACTOR_TYPE_RECT);
    return HS_GAME_STATE_OBJECT_BEING_POSITIONED;
}

static juint returnToPreviousStateFromChooseObject(
        SBStateMachine * stateMachine)
{
    herdingSheepsEngine * hEng = stateMachine->context;
    if (!hEng->objectActorList)
    {
        setTextToChooseObject();
        return HS_GAME_STATE_CHOOSE_OBJECT;
    }

    switch (herdingSheepsEngineGetFocussedObjectType(
                stateMachine->context))
    {
        case OBJECT_ACTOR_TYPE_POINT:
        case OBJECT_ACTOR_TYPE_H_LINE:
        case OBJECT_ACTOR_TYPE_V_LINE:
        case OBJECT_ACTOR_TYPE_RECT:
        {
            setTextToChooseVelocity();
            return HS_GAME_STATE_OBJECT_CHOOSE_VELOCITY;
        }
        case OBJECT_ACTOR_TYPE_UNSET:
        {
            break;
        }
    }
    return HS_GAME_STATE_ERROR;
}

static juint returnToPreviousStateFromObjectChooseDimension(
        SBStateMachine * stateMachine)
{
    herdingSheepsEngine * hEng = stateMachine->context;
    hEng->objectActorList->val->ptr.la->ca.shape.line.length = 30;

    switch (herdingSheepsEngineGetFocussedObjectType(
                stateMachine->context))
    {
        case OBJECT_ACTOR_TYPE_H_LINE:
        {
            setTextToAddObjectHLine();
            return HS_GAME_STATE_OBJECT_BEING_POSITIONED;
        }
        case OBJECT_ACTOR_TYPE_V_LINE:
        {
            setTextToAddObjectVLine();
            return HS_GAME_STATE_OBJECT_BEING_POSITIONED;
        }
        case OBJECT_ACTOR_TYPE_RECT:
        {
            setTextToAddObjectRect();
            return HS_GAME_STATE_OBJECT_BEING_POSITIONED;
        }
        case OBJECT_ACTOR_TYPE_POINT:
        case OBJECT_ACTOR_TYPE_UNSET:
        {
            break;
        }
    }
    return HS_GAME_STATE_ERROR;
}

static juint returnToPreviousStateFromObjectChooseVelocity(
        SBStateMachine * stateMachine)
{
    herdingSheepsEngine * hEng = stateMachine->context;
    jintVecScaled v = {.v = {{0,0}}, .s = 80};
    objectActorSetVelocity(
            hEng->objectActorList->val,
            &v);

    switch (herdingSheepsEngineGetFocussedObjectType(
                stateMachine->context))
    {
        case OBJECT_ACTOR_TYPE_H_LINE:
        case OBJECT_ACTOR_TYPE_V_LINE:
        case OBJECT_ACTOR_TYPE_RECT:
        {
            setTextToChooseDimensions();
            return HS_GAME_STATE_OBJECT_CHOOSE_DIMENSION;
        }
        case OBJECT_ACTOR_TYPE_POINT:
        {
            setTextToAddObjectPoint();
            return HS_GAME_STATE_OBJECT_BEING_POSITIONED;
        }
        case OBJECT_ACTOR_TYPE_UNSET:
        {
            break;
        }
    }
    return HS_GAME_STATE_ERROR;
}

static juint returnToPreviousStateFromObjectBeingPositioned(
        SBStateMachine * stateMachine)
{
    herdingSheepsEnginePopAndReleaseObject(
                stateMachine->context);

    setTextToChooseObject();
    return HS_GAME_STATE_CHOOSE_OBJECT;
}

juint returnToPreviousState(SBStateMachine * stateMachine, juint token)
{
    HS_GAME_STATE currentState;
    SBStateMachineGetCurrentState(stateMachine, &currentState);

    if (currentState == HS_GAME_STATE_CHOOSE_OBJECT)
    {
        return returnToPreviousStateFromChooseObject(
                stateMachine);
    }
    else if (currentState == HS_GAME_STATE_OBJECT_CHOOSE_DIMENSION)
    {
        return returnToPreviousStateFromObjectChooseDimension(
                stateMachine);
    }
    else if (currentState == HS_GAME_STATE_OBJECT_CHOOSE_VELOCITY)
    {
        return returnToPreviousStateFromObjectChooseVelocity(
                stateMachine);
    }
    else if (currentState == HS_GAME_STATE_OBJECT_BEING_POSITIONED)
    {
        return returnToPreviousStateFromObjectBeingPositioned(
                stateMachine);
    }

    return HS_GAME_STATE_ERROR;
}

static juint goToChooseObject(SBStateMachine * stateMachine, juint token)
{
    setTextToChooseObject();
    return HS_GAME_STATE_CHOOSE_OBJECT;
}

static juint goToObjectChooseDimensions(
        SBStateMachine * stateMachine, juint token)
{
    setTextToChooseDimensions();
    return HS_GAME_STATE_OBJECT_CHOOSE_DIMENSION;
}

static juint goToObjectChooseVelocity(
        SBStateMachine * stateMachine, juint token)
{
    setTextToChooseVelocity();
    return HS_GAME_STATE_OBJECT_CHOOSE_VELOCITY;
}

static juint objectBeenPositioned(SBStateMachine * stateMachine, juint token)
{
    switch (herdingSheepsEngineGetFocussedObjectType(
                stateMachine->context))
    {
        case OBJECT_ACTOR_TYPE_POINT:
            return goToObjectChooseVelocity(stateMachine, token);
        case OBJECT_ACTOR_TYPE_V_LINE:
        case OBJECT_ACTOR_TYPE_H_LINE:
        case OBJECT_ACTOR_TYPE_RECT:
            return goToObjectChooseDimensions(
                    stateMachine, token);
        default:
            break;
    }
    return HS_GAME_STATE_ERROR;
}

static juint goToRunning(SBStateMachine * stateMachine, juint token)
{
    setTextToRunning();
    engineUnpause(
            ((herdingSheepsEngine *)stateMachine->context)->engine);
    return HS_GAME_STATE_RUNNING;
}

static juint goToRunningInitial(SBStateMachine * stateMachine, juint token)
{
    herdingSheepsEngineCalculateFocussedObjectCollisionPoints(
            stateMachine->context);
    return goToRunning(stateMachine, token);
}

static juint goToPaused(SBStateMachine * stateMachine, juint token)
{
    setTextToPaused();
    enginePause(
            ((herdingSheepsEngine *)stateMachine->context)->engine);
    return HS_GAME_STATE_PAUSED;
}

static juint skipForward(SBStateMachine * stateMachine, juint token)
{
    HS_GAME_STATE currentState;
    SBStateMachineGetCurrentState(stateMachine, &currentState);

    engineAdvanceFrames(
        ((herdingSheepsEngine *)stateMachine->context)->engine, 1);
    return currentState;
}

static juint skipForward10(SBStateMachine * stateMachine, juint token)
{
    HS_GAME_STATE currentState;
    SBStateMachineGetCurrentState(stateMachine, &currentState);

    engineAdvanceFrames(
        ((herdingSheepsEngine *)stateMachine->context)->engine, 10);
    return currentState;
}

void HSStateMachineProcessInput(SBStateMachine * stateMachine)
{
    if (isStateActive(KEYPRESS_STATE_P))
    {
        SBStateMachineProcessInput(stateMachine, HS_GAME_STATE_TOKEN_P, NULL);
        deactivateState(KEYPRESS_STATE_P);
    }
    else if (isStateActive(KEYPRESS_STATE_V))
    {
        SBStateMachineProcessInput(stateMachine, HS_GAME_STATE_TOKEN_V, NULL);
        deactivateState(KEYPRESS_STATE_V);
    }
    else if (isStateActive(KEYPRESS_STATE_H))
    {
        SBStateMachineProcessInput(stateMachine, HS_GAME_STATE_TOKEN_H, NULL);
        deactivateState(KEYPRESS_STATE_H);
    }
    else if (isStateActive(KEYPRESS_STATE_R))
    {
        SBStateMachineProcessInput(stateMachine, HS_GAME_STATE_TOKEN_R, NULL);
        deactivateState(KEYPRESS_STATE_R);
    }
    else if (isStateActive(KEYPRESS_STATE_ESC))
    {
        SBStateMachineProcessInput(stateMachine, HS_GAME_STATE_TOKEN_ESC, NULL);
        deactivateState(KEYPRESS_STATE_ESC);
    }
    else if (isStateActive(KEYPRESS_STATE_SPACE))
    {
        SBStateMachineProcessInput(
                stateMachine, HS_GAME_STATE_TOKEN_SPACE, NULL);
        deactivateState(KEYPRESS_STATE_SPACE);
    }
    else if (isStateActive(KEYPRESS_STATE_RIGHT))
    {
        if (isStateActive(KEYPRESS_STATE_CTRL))
        {
            SBStateMachineProcessInput(
                    stateMachine,
                    HS_GAME_STATE_TOKEN_RIGHT_MODIFIED, NULL);
        }
        else
        {
            SBStateMachineProcessInput(
                    stateMachine, HS_GAME_STATE_TOKEN_RIGHT, NULL);
        }
        deactivateState(KEYPRESS_STATE_RIGHT);
    }
    
}

void HSStateMachineMouseCB(jint x, jint y, void * owner)
{
    SBStateMachine * stateMachine = owner;
    SBStateMachineProcessInput(stateMachine, HS_GAME_STATE_TOKEN_L_CLICK, NULL);
}

SBStateMachine * createHSStateMachine(herdingSheepsEngine * eng)
{
    keyStateBinding ksb;

    // TODO add function like 'getFreeState' to get unused state from inputProcessor
    // so then don't need to share keypress states with engine
    ksb.k = SDLK_p;
    ksb.s = KEYPRESS_STATE_P;
    ksb.t = BINDING_ONE_TIME;
    addBinding(&ksb);

    ksb.k = SDLK_v;
    ksb.s = KEYPRESS_STATE_V;
    ksb.t = BINDING_ONE_TIME;
    addBinding(&ksb);

    ksb.k = SDLK_h;
    ksb.s = KEYPRESS_STATE_H;
    ksb.t = BINDING_ONE_TIME;
    addBinding(&ksb);

    ksb.k = SDLK_r;
    ksb.s = KEYPRESS_STATE_R;
    ksb.t = BINDING_ONE_TIME;
    addBinding(&ksb);

    ksb.k = SDLK_ESCAPE;
    ksb.s = KEYPRESS_STATE_ESC;
    ksb.t = BINDING_ONE_TIME;
    addBinding(&ksb);

    ksb.k = SDLK_SPACE;
    ksb.s = KEYPRESS_STATE_SPACE;
    ksb.t = BINDING_ONE_TIME;
    addBinding(&ksb);

    ksb.k = SDLK_RIGHT;
    ksb.s = KEYPRESS_STATE_RIGHT;
    ksb.t = BINDING_ONE_TIME;
    addBinding(&ksb);

    ksb.k = SDLK_LCTRL;
    ksb.s = KEYPRESS_STATE_CTRL;
    ksb.t = BINDING_CONTINUOUS;
    addBinding(&ksb);

    SBStateMachine * stateMachine = createSBStateMachine(eng);
    mouseCallbackBinding mcb;
    mcb.type = SDL_MOUSEBUTTONDOWN;
    mcb.button = SDL_BUTTON_LEFT;
    mcb.callback = HSStateMachineMouseCB;
    mcb.owner = stateMachine;
    addMouseCallback(&mcb);

    juint ret = SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_CHOOSE_OBJECT, 6,
            HS_GAME_STATE_TOKEN_P, pointObjectChosen,
            HS_GAME_STATE_TOKEN_V, VLineObjectChosen,
            HS_GAME_STATE_TOKEN_H, HLineObjectChosen,
            HS_GAME_STATE_TOKEN_R, rectObjectChosen,
            HS_GAME_STATE_TOKEN_SPACE, goToRunningInitial,
            HS_GAME_STATE_TOKEN_ESC, returnToPreviousState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    ret = SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_OBJECT_BEING_POSITIONED, 2,
            HS_GAME_STATE_TOKEN_L_CLICK, objectBeenPositioned,
            HS_GAME_STATE_TOKEN_ESC, returnToPreviousState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    ret = SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_OBJECT_CHOOSE_DIMENSION, 2,
            HS_GAME_STATE_TOKEN_L_CLICK, goToObjectChooseVelocity,
            HS_GAME_STATE_TOKEN_ESC, returnToPreviousState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    ret = SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_OBJECT_CHOOSE_VELOCITY, 2,
            HS_GAME_STATE_TOKEN_L_CLICK, goToChooseObject,
            HS_GAME_STATE_TOKEN_ESC, returnToPreviousState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    ret = SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_RUNNING, 2,
            HS_GAME_STATE_TOKEN_SPACE, goToPaused,
            HS_GAME_STATE_TOKEN_ESC, returnToPreviousState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    ret = SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_PAUSED, 3,
            HS_GAME_STATE_TOKEN_SPACE, goToRunning,
            HS_GAME_STATE_TOKEN_RIGHT, skipForward,
            HS_GAME_STATE_TOKEN_RIGHT_MODIFIED, skipForward10);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }


    SBStateMachineSetCurrentState(stateMachine, HS_GAME_STATE_CHOOSE_OBJECT);
    return stateMachine;
}
