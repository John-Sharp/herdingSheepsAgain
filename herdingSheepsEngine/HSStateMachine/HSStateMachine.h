#ifndef HS_STATE_MACHINE_H
#define HS_STATE_MACHINE_H

#include "../herdingSheepsEngine.h"

void HSStateMachineProcessInput(SBStateMachine * stateMachine);
SBStateMachine * createHSStateMachine(herdingSheepsEngine * eng);

#endif

