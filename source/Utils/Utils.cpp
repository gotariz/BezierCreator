#include "Utils.h"

Vector2 Utils::lerp(float t, Vector2 p1, Vector2 p2)
{
    Vector2 result = p2 - p1;
    result *= t;
    return result + p1;
}

int Utils::roundNearest(int numToRound, int multiple) {
    int remainder = numToRound % multiple;

    if (remainder < 0) {
        if (remainder >= -(multiple / 2) )  return numToRound - remainder;
        else                                return numToRound - (multiple + remainder);
    } else if (remainder > 0) {
        if (remainder >= (multiple / 2) )   return numToRound + (multiple - remainder);
        else                                return numToRound - remainder;
    } else {
        return numToRound; // already a multiple
    }
}

int Utils::roundNearestf(double numToRound, int multiple) {

    double remainder = fmod(numToRound,multiple);
    double half = static_cast<double>(multiple)/2.0;
    int value = static_cast<int>(numToRound);

    if (remainder >= 0 && remainder >= half)        return value + multiple;
    else if (remainder < 0 && remainder <= -half)   return value - multiple;

    return value;
}

int Utils::roundUp(int numToRound, int multiple)
{
    if (multiple == 0)
        return numToRound;

    int remainder = abs(numToRound) % multiple;
    if (remainder == 0)
        return numToRound;
    if (numToRound < 0)
        return -(abs(numToRound) - remainder);

    return numToRound + multiple - remainder;
}

double Utils::roundNearestDecimal(double numToRound, int decimals)
{
    double multiplier = pow(10,decimals);

    //roundf(val * 100) / 100;

    numToRound *= multiplier;
    numToRound = roundNearestf(numToRound,1);
    return numToRound / multiplier;
}
