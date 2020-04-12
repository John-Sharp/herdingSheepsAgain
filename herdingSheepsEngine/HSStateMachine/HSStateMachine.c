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


juint pointObjectChosen(SBStateMachine * stateMachine, juint token)
{
    setTextToAddMainObjectPoint();
    herdingSheepsEngineSwitchMainObject(stateMachine->context, OBJECT_ACTOR_TYPE_POINT);
    return HS_GAME_STATE_MAIN_OBJECT_POINT;
}

juint vLineObjectChosen(SBStateMachine * stateMachine, juint token)
{
    setTextToAddMainObjectVLine();
    herdingSheepsEngineSwitchMainObject(stateMachine->context, OBJECT_ACTOR_TYPE_V_LINE);
    return HS_GAME_STATE_MAIN_OBJECT_V_LINE;
}

juint hLineObjectChosen(SBStateMachine * stateMachine, juint token)
{
    setTextToAddMainObjectHLine();
    herdingSheepsEngineSwitchMainObject(stateMachine->context, OBJECT_ACTOR_TYPE_H_LINE);
    return HS_GAME_STATE_MAIN_OBJECT_H_LINE;
}

static juint rectObjectChosen(SBStateMachine * stateMachine, juint token)
{
    setTextToAddMainObjectRect();
    herdingSheepsEngineSwitchMainObject(stateMachine->context, OBJECT_ACTOR_TYPE_RECT);
    return HS_GAME_STATE_MAIN_OBJECT_RECT;
}

static juint otherPointObjectChosen(SBStateMachine * stateMachine, juint token)
{
    setTextToAddOtherObjectPoint();
    herdingSheepsEnginePushOtherObject(stateMachine->context, OBJECT_ACTOR_TYPE_POINT);
    return HS_GAME_STATE_OTHER_OBJECT_BEING_POSITIONED;
}

static juint otherVLineObjectChosen(SBStateMachine * stateMachine, juint token)
{
    setTextToAddOtherObjectVLine();
    herdingSheepsEnginePushOtherObject(stateMachine->context, OBJECT_ACTOR_TYPE_V_LINE);
    return HS_GAME_STATE_OTHER_OBJECT_BEING_POSITIONED;
}

static juint otherHLineObjectChosen(SBStateMachine * stateMachine, juint token)
{
    setTextToAddOtherObjectHLine();
    herdingSheepsEnginePushOtherObject(stateMachine->context, OBJECT_ACTOR_TYPE_H_LINE);
    return HS_GAME_STATE_OTHER_OBJECT_BEING_POSITIONED;
}

static juint otherRectObjectChosen(SBStateMachine * stateMachine, juint token)
{
    setTextToAddOtherObjectRect();
    herdingSheepsEnginePushOtherObject(stateMachine->context, OBJECT_ACTOR_TYPE_RECT);
    return HS_GAME_STATE_OTHER_OBJECT_BEING_POSITIONED;
}

juint goToChooseVelocityMainObject(SBStateMachine * stateMachine, juint token)
{
    setTextToChooseVelocity();
    return HS_GAME_STATE_MAIN_OBJECT_CHOOSE_VELOCITY;
}

static juint goToChooseDimensionsMainObject(SBStateMachine * stateMachine, juint token)
{
    setTextToChooseDimensions();
    return HS_GAME_STATE_MAIN_OBJECT_CHOOSE_DIMENSION;
}

juint returnToMainState(SBStateMachine * stateMachine, juint token)
{
    setTextToAddMainObject();
    herdingSheepsEngineSwitchMainObject(stateMachine->context, OBJECT_ACTOR_TYPE_UNSET);
    return HS_GAME_STATE_CHOOSE_MAIN_OBJECT;
}

static juint returnToPreviousStateFromChooseOtherObject(
        SBStateMachine * stateMachine)
{
    herdingSheepsEngine * hEng = stateMachine->context;
    if (!hEng->otherActorList)
    {
        setTextToChooseVelocity();
        return HS_GAME_STATE_MAIN_OBJECT_CHOOSE_VELOCITY;
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
            return HS_GAME_STATE_OTHER_OBJECT_CHOOSE_VELOCITY;
        }
        case OBJECT_ACTOR_TYPE_UNSET:
        {
            break;
        }
    }
    return HS_GAME_STATE_ERROR;
}

static juint returnToPreviousStateFromOtherObjectChooseDimension(
        SBStateMachine * stateMachine)
{
    herdingSheepsEngine * hEng = stateMachine->context;
    hEng->otherActorList->val->ptr.la->ca.shape.line.length = 30;

    switch (herdingSheepsEngineGetFocussedObjectType(
                stateMachine->context))
    {
        case OBJECT_ACTOR_TYPE_H_LINE:
        {
            setTextToAddOtherObjectHLine();
            return HS_GAME_STATE_OTHER_OBJECT_BEING_POSITIONED;
        }
        case OBJECT_ACTOR_TYPE_V_LINE:
        {
            setTextToAddOtherObjectVLine();
            return HS_GAME_STATE_OTHER_OBJECT_BEING_POSITIONED;
        }
        case OBJECT_ACTOR_TYPE_RECT:
        {
            setTextToAddOtherObjectRect();
            return HS_GAME_STATE_OTHER_OBJECT_BEING_POSITIONED;
        }
        case OBJECT_ACTOR_TYPE_POINT:
        case OBJECT_ACTOR_TYPE_UNSET:
        {
            break;
        }
    }
    return HS_GAME_STATE_ERROR;
}

static juint returnToPreviousStateFromOtherObjectChooseVelocity(
        SBStateMachine * stateMachine)
{
    herdingSheepsEngine * hEng = stateMachine->context;
    jintVecScaled v = {.v = {{0,0}}, .s = 80};
    objectActorSetVelocity(
            hEng->otherActorList->val,
            &v);

    switch (herdingSheepsEngineGetFocussedObjectType(
                stateMachine->context))
    {
        case OBJECT_ACTOR_TYPE_H_LINE:
        case OBJECT_ACTOR_TYPE_V_LINE:
        case OBJECT_ACTOR_TYPE_RECT:
        {
            setTextToChooseDimensions();
            return HS_GAME_STATE_OTHER_OBJECT_CHOOSE_DIMENSION;
        }
        case OBJECT_ACTOR_TYPE_POINT:
        {
            setTextToAddOtherObjectPoint();
            return HS_GAME_STATE_OTHER_OBJECT_BEING_POSITIONED;
        }
        case OBJECT_ACTOR_TYPE_UNSET:
        {
            break;
        }
    }
    return HS_GAME_STATE_ERROR;
}

static juint returnToPreviousStateFromOtherObjectBeingPositioned(
        SBStateMachine * stateMachine)
{
    if (!herdingSheepsEnginePopAndReleaseOtherObject(
                stateMachine->context))
    {
        setTextToChooseVelocity();
        return HS_GAME_STATE_MAIN_OBJECT_CHOOSE_VELOCITY;
    }

    setTextToChooseOtherObject();
    return HS_GAME_STATE_CHOOSE_OTHER_OBJECT;
}

static juint returnToPreviousStateFromMainObjectChooseVelocity(
        SBStateMachine * stateMachine)
{

    jintVecScaled v = {{{0,0}},80};
    objectActorSetVelocity(
            &((herdingSheepsEngine *)stateMachine->context)->mainActor,
            &v);

    switch (herdingSheepsEngineGetMainObjectType(
                stateMachine->context))
    {
        case OBJECT_ACTOR_TYPE_POINT:
        {
            setTextToAddMainObjectPoint();
            return HS_GAME_STATE_MAIN_OBJECT_POINT;
        }
        case OBJECT_ACTOR_TYPE_V_LINE:
        case OBJECT_ACTOR_TYPE_H_LINE:
        case OBJECT_ACTOR_TYPE_RECT:
        {
            setTextToChooseDimensions();
            return HS_GAME_STATE_MAIN_OBJECT_CHOOSE_DIMENSION;
        }
        default:
        {
            break;
        }
    }
    return HS_GAME_STATE_ERROR;
}

juint returnToPreviousState(SBStateMachine * stateMachine, juint token)
{
    HS_GAME_STATE currentState;
    SBStateMachineGetCurrentState(stateMachine, &currentState);

    if (currentState == HS_GAME_STATE_MAIN_OBJECT_CHOOSE_DIMENSION)
    {
        herdingSheepsEngineSwitchMainObject(
                stateMachine->context,
                herdingSheepsEngineGetMainObjectType(stateMachine->context));
        switch (herdingSheepsEngineGetMainObjectType(stateMachine->context))
        {
            case OBJECT_ACTOR_TYPE_V_LINE:
                setTextToAddMainObjectVLine();
                return HS_GAME_STATE_MAIN_OBJECT_V_LINE;
            case OBJECT_ACTOR_TYPE_H_LINE:
                setTextToAddMainObjectHLine();
                return HS_GAME_STATE_MAIN_OBJECT_H_LINE;
            case OBJECT_ACTOR_TYPE_RECT:
                setTextToAddMainObjectRect();
                return HS_GAME_STATE_MAIN_OBJECT_RECT;
            default:
                break;
        }
    }
    else if (currentState == HS_GAME_STATE_MAIN_OBJECT_CHOOSE_VELOCITY)
    {
        return returnToPreviousStateFromMainObjectChooseVelocity(
                stateMachine);
    }
    else if (currentState == HS_GAME_STATE_CHOOSE_OTHER_OBJECT)
    {
        return returnToPreviousStateFromChooseOtherObject(
                stateMachine);
    }
    else if (currentState == HS_GAME_STATE_OTHER_OBJECT_CHOOSE_DIMENSION)
    {
        return returnToPreviousStateFromOtherObjectChooseDimension(
                stateMachine);
    }
    else if (currentState == HS_GAME_STATE_OTHER_OBJECT_CHOOSE_VELOCITY)
    {
        return returnToPreviousStateFromOtherObjectChooseVelocity(
                stateMachine);
    }
    else if (currentState == HS_GAME_STATE_OTHER_OBJECT_BEING_POSITIONED)
    {
        return returnToPreviousStateFromOtherObjectBeingPositioned(
                stateMachine);
    }

    switch (herdingSheepsEngineGetMainObjectType(stateMachine->context))
    {
        case OBJECT_ACTOR_TYPE_POINT:
            setTextToAddMainObjectPoint();
            return HS_GAME_STATE_MAIN_OBJECT_POINT;
        case OBJECT_ACTOR_TYPE_V_LINE:
        case OBJECT_ACTOR_TYPE_H_LINE:
            setTextToChooseDimensions();
            return HS_GAME_STATE_MAIN_OBJECT_CHOOSE_DIMENSION;
        default:
            break;
    }

    return HS_GAME_STATE_MAIN_OBJECT_POINT;
}

static juint goToChooseOtherObject(SBStateMachine * stateMachine, juint token)
{
    setTextToChooseOtherObject();
    return HS_GAME_STATE_CHOOSE_OTHER_OBJECT;
}

static juint goToOtherObjectChooseDimensions(
        SBStateMachine * stateMachine, juint token)
{
    setTextToChooseDimensions();
    return HS_GAME_STATE_OTHER_OBJECT_CHOOSE_DIMENSION;
}

static juint goToOtherObjectChooseVelocity(
        SBStateMachine * stateMachine, juint token)
{
    setTextToChooseVelocity();
    return HS_GAME_STATE_OTHER_OBJECT_CHOOSE_VELOCITY;
}

static juint otherObjectBeenPositioned(SBStateMachine * stateMachine, juint token)
{
    switch (herdingSheepsEngineGetFocussedObjectType(
                stateMachine->context))
    {
        case OBJECT_ACTOR_TYPE_POINT:
            return goToOtherObjectChooseVelocity(stateMachine, token);
        case OBJECT_ACTOR_TYPE_V_LINE:
        case OBJECT_ACTOR_TYPE_H_LINE:
        case OBJECT_ACTOR_TYPE_RECT:
            return goToOtherObjectChooseDimensions(
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
    herdingSheepsEngineCalculateMainObjectCollisionPoints(
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

    juint ret =  SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_CHOOSE_MAIN_OBJECT, 4,
            HS_GAME_STATE_TOKEN_P, pointObjectChosen,
            HS_GAME_STATE_TOKEN_V, vLineObjectChosen,
            HS_GAME_STATE_TOKEN_H, hLineObjectChosen,
            HS_GAME_STATE_TOKEN_R, rectObjectChosen);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }
    ret =  SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_MAIN_OBJECT_H_LINE, 2,
            HS_GAME_STATE_TOKEN_L_CLICK, goToChooseDimensionsMainObject,
            HS_GAME_STATE_TOKEN_ESC, returnToMainState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    ret =  SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_MAIN_OBJECT_V_LINE, 2,
            HS_GAME_STATE_TOKEN_L_CLICK, goToChooseDimensionsMainObject,
            HS_GAME_STATE_TOKEN_ESC, returnToMainState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    ret =  SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_MAIN_OBJECT_POINT, 2,
            HS_GAME_STATE_TOKEN_L_CLICK, goToChooseVelocityMainObject,
            HS_GAME_STATE_TOKEN_ESC, returnToMainState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    ret =  SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_MAIN_OBJECT_RECT, 2,
            HS_GAME_STATE_TOKEN_L_CLICK, goToChooseDimensionsMainObject,
            HS_GAME_STATE_TOKEN_ESC, returnToMainState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    ret = SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_MAIN_OBJECT_CHOOSE_VELOCITY, 2,
            HS_GAME_STATE_TOKEN_L_CLICK, goToChooseOtherObject,
            HS_GAME_STATE_TOKEN_ESC, returnToPreviousState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    ret = SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_MAIN_OBJECT_CHOOSE_DIMENSION, 2,
            HS_GAME_STATE_TOKEN_L_CLICK, goToChooseVelocityMainObject,
            HS_GAME_STATE_TOKEN_ESC, returnToPreviousState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    ret = SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_CHOOSE_OTHER_OBJECT, 6,
            HS_GAME_STATE_TOKEN_P, otherPointObjectChosen,
            HS_GAME_STATE_TOKEN_V, otherVLineObjectChosen,
            HS_GAME_STATE_TOKEN_H, otherHLineObjectChosen,
            HS_GAME_STATE_TOKEN_R, otherRectObjectChosen,
            HS_GAME_STATE_TOKEN_SPACE, goToRunningInitial,
            HS_GAME_STATE_TOKEN_ESC, returnToPreviousState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    ret = SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_OTHER_OBJECT_BEING_POSITIONED, 2,
            HS_GAME_STATE_TOKEN_L_CLICK, otherObjectBeenPositioned,
            HS_GAME_STATE_TOKEN_ESC, returnToPreviousState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    ret = SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_OTHER_OBJECT_CHOOSE_DIMENSION, 2,
            HS_GAME_STATE_TOKEN_L_CLICK, goToOtherObjectChooseVelocity,
            HS_GAME_STATE_TOKEN_ESC, returnToPreviousState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    ret = SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_OTHER_OBJECT_CHOOSE_VELOCITY, 2,
            HS_GAME_STATE_TOKEN_L_CLICK, goToChooseOtherObject,
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


    SBStateMachineSetCurrentState(stateMachine, HS_GAME_STATE_CHOOSE_MAIN_OBJECT);
    return stateMachine;
}
