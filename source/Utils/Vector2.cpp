#include "Vector2.h"

Vector2::Vector2(const char* vectorString)
{
    if (vectorString == NULL)
    {
        x = 0;
        y = 0;
    }
    else
    {
        std::string value = "";
        std::string vstring = vectorString;
        for (char c: vstring)
        {
            if (c == ',')
            {
                x = atof(value.c_str());
                value = "";
            }
            else
            {
                value += gz::toString(c);
            }
        }

        y = atof(value.c_str());
    }
}

Vector2::Vector2(double xval, double yval) : x(xval), y(yval)
{

}

Vector2::~Vector2()
{
    //dtor
}

///////////////////////////////////////////////////////////////////////////
// getters
///////////////////////////////////////////////////////////////////////////

double Vector2::getAngle()
{
    double a = atan2(y,x) * RADTODEG;
    if (a < 0) a += 360;
    return a;
}

double Vector2::getMagnitude()
{
    return sqrt( (x * x) + (y * y) );
}

Vector2 Vector2::getNormalised()
{
    Vector2 normalisedVector = Vector2(x,y);
    normalisedVector.normalise();

    return normalisedVector;
}

sf::Vector2f Vector2::toSfmlVector()
{
    return sf::Vector2f(x,y);
}

///////////////////////////////////////////////////////////////////////////
// setters =SUM( B5 * =SIN(RADIANS(36)) - B6 * = SIN(RADIANS(36))      )
///////////////////////////////////////////////////////////////////////////

void Vector2::print(std::string name)
{
    gz::print_d(name+  ": {x=" + gz::toString(x) + ",y=" + gz::toString(y) + "}");
}

string Vector2::toString()
{
    return gz::toString(x) + "," + gz::toString(y);
}


double Vector2::dot(Vector2 v2)
{
    Vector2 v1(x,y);

    v1.normalise();
    v2.normalise();

    return acos((v1.x * v2.x) + (v1.y * v2.y)) * RADTODEG;
}

void Vector2::rotate(double angle)
{
    double theta = (angle * DEGTORAD);
    double cs = cos(theta);
    double sn = sin(theta);

    double newx = x * cs - y * sn;
    double newy = x * sn + y * cs;

    set(newx,newy);
}

void Vector2::rotateAround(double angle, Vector2 vec)
{
    rotateAround(angle, vec.x, vec.y);
}

void Vector2::rotateAround(double angle, double origin_x, double origin_y)
{
    double px = x - origin_x;
    double py = y - origin_y;

    double theta = (angle * DEGTORAD);
    double cs = cos(theta);
    double sn = sin(theta);

    double newx = (px * cs - py * sn) + origin_x;
    double newy = (px * sn + py * cs) + origin_y;

    set(newx,newy);
}

void Vector2::setMagnitude(double magnitude)
{
    normalise();
    x *= magnitude;
    y *= magnitude;
}

void Vector2::normalise()
{
    double magnitude = getMagnitude();
    if (magnitude > 0)
    {
        x /= magnitude;
        y /= magnitude;
    }
}

void Vector2::set(double xval, double yval)
{
    x = xval;
    y = yval;
}

void Vector2::set(Vector2 vectorToCopy)
{
    x = vectorToCopy.x;
    y = vectorToCopy.y;
}


///////////////////////////////////////////////////////////////////////////
// operators
///////////////////////////////////////////////////////////////////////////

void Vector2::operator=(Vector2 vec)
{
    x = vec.x;
    y = vec.y;
}

bool Vector2::operator==(Vector2 vec)
{
    return (x == vec.x and y == vec.y);
}

bool Vector2::operator!=(Vector2 vec)
{
    return (x != vec.x or y != vec.y);
}

Vector2 Vector2::operator+(Vector2 vec)
{
    return Vector2( x + vec.x, y + vec.y );
}

Vector2 Vector2::operator-(Vector2 vec)
{
    return Vector2( x - vec.x, y - vec.y );
}

Vector2 Vector2::operator*(Vector2 vec)
{
    return Vector2(x * vec.x, y * vec.y);
}

Vector2 Vector2::operator/(Vector2 vec)
{
    return Vector2(x / vec.x, y / vec.y);
}

void Vector2::operator+=(Vector2 vec)
{
    x += vec.x;
    y += vec.y;
}

void Vector2::operator-=(Vector2 vec)
{
    x -= vec.x;
    y -= vec.y;
}

void Vector2::operator*=(Vector2 vec)
{
    x *= vec.x;
    y *= vec.y;
}

void Vector2::operator/=(Vector2 vec)
{
    x /= vec.x;
    y /= vec.y;
}


Vector2 Vector2::operator+(double scalar)
{
    return Vector2(x + scalar, y + scalar);
}

Vector2 Vector2::operator-(double scalar)
{
    return Vector2(x - scalar, y - scalar);
}

Vector2 Vector2::operator*(double scalar)
{
    return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator/(double scalar)
{
    return Vector2(x / scalar, y / scalar);
}

void Vector2::operator+=(double scalar)
{
    x += scalar;
    y += scalar;
}

void Vector2::operator-=(double scalar)
{
    x -= scalar;
    y -= scalar;
}

void Vector2::operator*=(double scalar)
{
    x *= scalar;
    y *= scalar;
}

void Vector2::operator/=(double scalar)
{
    x /= scalar;
    y /= scalar;
}







