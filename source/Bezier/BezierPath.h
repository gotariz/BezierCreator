#ifndef BEZIERPATH_H
#define BEZIERPATH_H

#include "general.h"
#include "Utils/Vector2.h"
#include <cmath>

struct BPoint
{
    Vector2 p;
    Vector2 h1;
    Vector2 h2;

    void setPosition(Vector2 pos)
    {
        Vector2 h1Delta = h1 - p;
        Vector2 h2Delta = h2 - p;
        p = pos;
        h1 = h1Delta + pos;
        h2 = h2Delta + pos;
    }

    void setH1Linked(float x, float y)
    {
        setH1Linked(Vector2(x,y));
    }

    void setH2Linked(float x, float y)
    {
        setH2Linked(Vector2(x,y));
    }

    void setH1Linked(Vector2 pos)
    {
        h1 = pos;
        Vector2 delta = pos - p;
        h2 = (delta * -1) + p;
    }

    void setH2Linked(Vector2 pos)
    {
        h2 = pos;
        Vector2 delta = pos - p;
        h1 = (delta * -1) + p;
    }

    BPoint(Vector2 point) : BPoint(point,point,point){}

    BPoint(Vector2 point, Vector2 handle1, Vector2 handle2)
    {
        p = point;
        h1 = handle1;
        h2 = handle2;
    }
};

class BezierPath
{
    public:
        BezierPath();
        virtual ~BezierPath();

        vector<BPoint> points;

        int getCurveCount();
        Vector2 getPoint(float t, int n);
        Vector2 getPoint(float t);

        Vector2 findPoint(Vector2 pos, float distance);
        Vector2 findPoint(float x, float y, float distance);

    private:
        float normalise(float number); // truncate the number to be between 0 and 1 inclusive
};

#endif // BEZIERPATH_H
