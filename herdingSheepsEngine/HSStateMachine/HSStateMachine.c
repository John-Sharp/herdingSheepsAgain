#include "HSStateMachine.h"
#include "../actors/textBoxActor/infoBox/infoBox.h"

typedef enum HS_GAME_STATE_TOKEN {
    HS_GAME_STATE_TOKEN_P,
    HS_GAME_STATE_TOKEN_V,
    HS_GAME_STATE_TOKEN_H,
    HS_GAME_STATE_TOKEN_L_CLICK,
    HS_GAME_STATE_TOKEN_ESC,
    HS_GAME_STATE_TOKEN_SPACE
} HS_GAME_STATE_TOKEN;


juint pointObjectChosen(SBStateMachine * stateMachine, juint token)
{
    setTextToAddMainObjectPoint();
    herdingSheepsEngineSwitchMainObject(stateMachine->context, MAIN_ACTOR_TYPE_POINT);
    return HS_GAME_STATE_MAIN_OBJECT_POINT;
}

juint vLineObjectChosen(SBStateMachine * stateMachine, juint token)
{
    setTextToAddMainObjectVLine();
    herdingSheepsEngineSwitchMainObject(stateMachine->context, MAIN_ACTOR_TYPE_V_LINE);
    return HS_GAME_STATE_MAIN_OBJECT_V_LINE;
}

juint hLineObjectChosen(SBStateMachine * stateMachine, juint token)
{
    setTextToAddMainObjectHLine();
    herdingSheepsEngineSwitchMainObject(stateMachine->context, MAIN_ACTOR_TYPE_H_LINE);
    return HS_GAME_STATE_MAIN_OBJECT_H_LINE;
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
    herdingSheepsEngineSwitchMainObject(stateMachine->context, MAIN_ACTOR_TYPE_UNSET);
    return HS_GAME_STATE_CHOOSE_MAIN_OBJECT;
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
            case MAIN_ACTOR_TYPE_V_LINE:
                setTextToAddMainObjectVLine();
                return HS_GAME_STATE_MAIN_OBJECT_V_LINE;
            case MAIN_ACTOR_TYPE_H_LINE:
                setTextToAddMainObjectHLine();
                return HS_GAME_STATE_MAIN_OBJECT_H_LINE;
            default:
                break;
        }
    }
    else if (currentState == HS_GAME_STATE_CHOOSE_OTHER_OBJECT)
    {
        setTextToChooseVelocity();
        return HS_GAME_STATE_MAIN_OBJECT_CHOOSE_VELOCITY;
    }

    switch (herdingSheepsEngineGetMainObjectType(stateMachine->context))
    {
        case MAIN_ACTOR_TYPE_POINT:
            setTextToAddMainObjectPoint();
            return HS_GAME_STATE_MAIN_OBJECT_POINT;
        case MAIN_ACTOR_TYPE_V_LINE:
        case MAIN_ACTOR_TYPE_H_LINE:
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

static juint goToRunning(SBStateMachine * stateMachine, juint token)
{
    setTextToRunning();
    return HS_GAME_STATE_RUNNING;
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
    else if (isStateActive(KEYPRESS_STATE_ESC))
    {
        SBStateMachineProcessInput(stateMachine, HS_GAME_STATE_TOKEN_ESC, NULL);
        deactivateState(KEYPRESS_STATE_ESC);
    }
    else if (isStateActive(KEYPRESS_STATE_SPACE))
    {
        SBStateMachineProcessInput(stateMachine, HS_GAME_STATE_TOKEN_SPACE, NULL);
        deactivateState(KEYPRESS_STATE_SPACE);
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

    ksb.k = SDLK_ESCAPE;
    ksb.s = KEYPRESS_STATE_ESC;
    ksb.t = BINDING_ONE_TIME;
    addBinding(&ksb);

    ksb.k = SDLK_SPACE;
    ksb.s = KEYPRESS_STATE_SPACE;
    ksb.t = BINDING_ONE_TIME;
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
            HS_GAME_STATE_CHOOSE_MAIN_OBJECT, 3,
            HS_GAME_STATE_TOKEN_P, pointObjectChosen,
            HS_GAME_STATE_TOKEN_V, vLineObjectChosen,
            HS_GAME_STATE_TOKEN_H, hLineObjectChosen);
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
            HS_GAME_STATE_CHOOSE_OTHER_OBJECT, 2,
            HS_GAME_STATE_TOKEN_SPACE, goToRunning,
            HS_GAME_STATE_TOKEN_ESC, returnToPreviousState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    ret = SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_RUNNING, 1,
            HS_GAME_STATE_TOKEN_ESC, returnToPreviousState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    SBStateMachineSetCurrentState(stateMachine, HS_GAME_STATE_CHOOSE_MAIN_OBJECT);
    return stateMachine;
}
