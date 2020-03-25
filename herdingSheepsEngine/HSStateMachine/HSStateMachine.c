#include "HSStateMachine.h"
#include "../actors/textBoxActor/infoBox/infoBox.h"

typedef enum HS_GAME_STATE {
    HS_GAME_STATE_CHOOSE_MAIN_OBJECT,
    HS_GAME_STATE_MAIN_OBJECT_H_LINE,
    HS_GAME_STATE_MAIN_OBJECT_V_LINE,
    HS_GAME_STATE_MAIN_OBJECT_POINT,
} HS_GAME_STATE;

typedef enum HS_GAME_STATE_TOKEN {
    HS_GAME_STATE_TOKEN_P,
    HS_GAME_STATE_TOKEN_V,
    HS_GAME_STATE_TOKEN_H,
    HS_GAME_STATE_TOKEN_ESC
} HS_GAME_STATE_TOKEN;


juint pointObjectChosen(SBStateMachine * stateMachine, juint token)
{
    setTextToAddMainObjectPoint();
    herdingSheepsEngineSwitchMainObject(stateMachine->context, MAIN_ACTOR_TYPE_POINT);
    return HS_GAME_STATE_MAIN_OBJECT_POINT;
}

juint vLineObjectChosen(SBStateMachine * stateMachine, juint token)
{
    printf("add main v-line object\n\n");
    return HS_GAME_STATE_MAIN_OBJECT_V_LINE;
}

juint hLineObjectChosen(SBStateMachine * stateMachine, juint token)
{
    printf("add main h-line object\n\n");
    return HS_GAME_STATE_MAIN_OBJECT_H_LINE;
}

juint returnToMainState(SBStateMachine * stateMachine, juint token)
{
    setTextToAddMainObject();
    herdingSheepsEngineSwitchMainObject(stateMachine->context, MAIN_ACTOR_TYPE_UNSET);
    return HS_GAME_STATE_CHOOSE_MAIN_OBJECT;
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

    SBStateMachine * stateMachine = createSBStateMachine(eng);
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
            HS_GAME_STATE_MAIN_OBJECT_H_LINE, 1,
            HS_GAME_STATE_TOKEN_ESC, returnToMainState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    ret =  SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_MAIN_OBJECT_V_LINE, 1,
            HS_GAME_STATE_TOKEN_ESC, returnToMainState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    ret =  SBStateMachineAddState(
            stateMachine,
            HS_GAME_STATE_MAIN_OBJECT_POINT, 1,
            HS_GAME_STATE_TOKEN_ESC, returnToMainState);
    if (ret != SB_STATE_MACHINE_OK)
    {
        printf("Failure after attempting to set up main state machine\n\n");
        exit(1);
    }

    SBStateMachineSetCurrentState(stateMachine, HS_GAME_STATE_CHOOSE_MAIN_OBJECT);
    return stateMachine;
}
