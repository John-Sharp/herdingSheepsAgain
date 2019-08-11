#include "engine/engine.h"
#include <jTypes.h>
#include <SDL2/SDL.h>

#include <cairo.h>
#include <math.h>

#include <stdio.h>

#include "herdingSheepsEngine/herdingSheepsEngine.h"

int main()
{
    herdingSheepsEngine eng;
    initHerdingSheepsEngine(&eng);

    engineStart(eng.engine);
}
