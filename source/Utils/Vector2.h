#ifndef VECTOR2_H
#define VECTOR2_H

#include "SFML/graphics.hpp"
#include "SFML/audio.hpp"
#include "SFML/window.hpp"
#include <cmath>
#include "gz.h"
#include "consts.h"

class Vector2
{
    public: //constructors & destructors
        Vector2(){}
        Vector2(const char* vectorString);
        Vector2(double xval, double yval);
        virtual ~Vector2();

    public: // variables
        double x = 0;
        double y = 0;

    public: // functions
        double   getMagnitude();
        Vector2 getNormalised();
        sf::Vector2f  toSfmlVector();
        double getAngle();

        double dot(Vector2 v2);

        void    rotate(double angle);
        void    rotateAround(double angle, Vector2 vec);
        void    rotateAround(double angle, double origin_x, double origin_y);
        void    normalise();
        void    setMagnitude(double magnitude);
        void    set(double xval, double yval);
        void    set(Vector2 vectorToCopy);

        string    toString();
        void    print(std::string name);

    public: // operators
        void operator=(Vector2 vec);
        bool operator==(Vector2 vec);
        bool operator!=(Vector2 vec);

        Vector2 operator+(Vector2 vec);
        Vector2 operator-(Vector2 vec);
        Vector2 operator*(Vector2 vec);
        Vector2 operator/(Vector2 vec);
        Vector2 operator+(double scalar);
        Vector2 operator-(double scalar);
        Vector2 operator*(double scalar);
        Vector2 operator/(double scalar);

        void operator+=(Vector2 vec);
        void operator-=(Vector2 vec);
        void operator*=(Vector2 vec);
        void operator/=(Vector2 vec);
        void operator+=(double scalar);
        void operator-=(double scalar);
        void operator*=(double scalar);
        void operator/=(double scalar);
};

#endif // VECTOR2_H
