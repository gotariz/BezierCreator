#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "general.h"
#include "Vector2.h"
#include <cmath>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

class Utils
{
    public:

        static Vector2  lerp(float t, Vector2 p1, Vector2 p2);

        static int      roundUp(int numToRound, int multiple);
        static int      roundNearest(int numToRound, int multiple);
        static int      roundNearestf(float numToRound, int multiple);
        static float    roundNearestDecimal(float numToRound, int multiple);

};

#endif // UTILS_H_INCLUDED
