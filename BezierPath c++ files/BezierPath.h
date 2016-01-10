#ifndef BEZIERPATH_H
#define BEZIERPATH_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>

#include <cmath>
#include <string>
#include <vector>
#include <algorithm>

#include <iostream>
#include <sstream>

namespace bz
{
    static const float PI = 3.1429;
    static const float RADTODEG = PI / 180.f;
    static const float DEGTORAD = 180.f / PI;

    //////////////////////////////////////////////////////////////////
    // Vector2
    //////////////////////////////////////////////////////////////////
    class Vector2
    {
        public:
            float x;
            float y;

            Vector2(float xval = 0, float yval = 0) : x(xval), y(yval) {}
            virtual ~Vector2(){}

            // functions
            float   getMagnitude();
            void    normalise();

            // operators
            void operator=(Vector2 vec);
            void operator+=(Vector2 vec);
            Vector2 operator+(Vector2 vec);//
            Vector2 operator-(Vector2 vec);//
            Vector2 operator*(float scalar);//
    };
    //////////////////////////////////////////////////////////////////
    // BPoint
    //////////////////////////////////////////////////////////////////
    class BPoint
    {
        public:
            BPoint(Vector2 point) : BPoint(point,point,point){}
            BPoint(Vector2 point, Vector2 handle1, Vector2 handle2)
            {
                p = point;
                h1 = handle1;
                h2 = handle2;
            }
            virtual ~BPoint(){}

            Vector2 p;
            Vector2 h1;
            Vector2 h2;

            void setPosition(Vector2 pos);
            void setH1Linked(float x, float y);
            void setH2Linked(float x, float y);
            void setH1Linked(Vector2 pos);
            void setH2Linked(Vector2 pos);
    };
    //////////////////////////////////////////////////////////////////
    // Bezier Path
    //////////////////////////////////////////////////////////////////
    class BezierPath
    {
        public:
            BezierPath(){}
            virtual ~BezierPath(){}

            std::vector<BPoint> points;

            int     getCurveCount();
            Vector2 getPoint(float t, int n);
            Vector2 getPoint(float t);
            Vector2 findPoint(Vector2 pos, float distance);
            Vector2 findPoint(float x, float y, float distance);

            bool loadPath(std::string path);

            std::vector<std::string> splitString(std::string path, char delimeter);

        private:
            float normalise(float number);
    };

}
#endif // BEZIERPATH_H
