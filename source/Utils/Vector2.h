#ifndef VECTOR2_H
#define VECTOR2_H

#include "SFML/graphics.hpp"
#include "SFML/audio.hpp"
#include "SFML/window.hpp"
#include <cmath>
#include "consts.h"
#include "gz.h"

using namespace std;

class Vector2
{
    public: //constructors & destructors
        Vector2(){}
        Vector2(const char* vectorString);
        Vector2(float xval, float yval);
        virtual ~Vector2();

    public: // variables
        float x = 0;
        float y = 0;

    public: // functions
        float      getMagnitude();
        Vector2     getNormalised();
        float      getAngle();

        float dot(Vector2 v2);

        void    rotate(float angle);
        void    rotateAround(float angle, Vector2 vec);
        void    rotateAround(float angle, float origin_x, float origin_y);
        void    normalise();
        void    setMagnitude(float magnitude);
        void    set(float xval, float yval);
        void    set(Vector2 vectorToCopy);

        string    toString();
        void        print(std::string name);

    public: // operators
        void operator=(Vector2 vec);
        bool operator==(Vector2 vec);
        bool operator!=(Vector2 vec);

        Vector2 operator+(Vector2 vec);
        Vector2 operator-(Vector2 vec);
        Vector2 operator*(Vector2 vec);
        Vector2 operator/(Vector2 vec);
        Vector2 operator+(float scalar);
        Vector2 operator-(float scalar);
        Vector2 operator*(float scalar);
        Vector2 operator/(float scalar);

        void operator+=(Vector2 vec);
        void operator-=(Vector2 vec);
        void operator*=(Vector2 vec);
        void operator/=(Vector2 vec);
        void operator+=(float scalar);
        void operator-=(float scalar);
        void operator*=(float scalar);
        void operator/=(float scalar);
};

#endif // VECTOR2_H
