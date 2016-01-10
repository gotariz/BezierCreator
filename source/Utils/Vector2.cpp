#include "Vector2.h"

Vector2::Vector2(float xval, float yval) : x(xval), y(yval)
{

}

Vector2::~Vector2()
{
    //dtor
}

///////////////////////////////////////////////////////////////////////////
// getters
///////////////////////////////////////////////////////////////////////////

float Vector2::getAngle()
{
    float a = atan2(y,x) * RADTODEG;
    if (a < 0) a += 360;
    return a;
}

float Vector2::getMagnitude()
{
    return sqrt( (x * x) + (y * y) );
}

Vector2 Vector2::getNormalised()
{
    Vector2 normalisedVector = Vector2(x,y);
    normalisedVector.normalise();

    return normalisedVector;
}

///////////////////////////////////////////////////////////////////////////
// setters =SUM( B5 * =SIN(RADIANS(36)) - B6 * = SIN(RADIANS(36))      )
///////////////////////////////////////////////////////////////////////////
string Vector2::toString()
{
    return gz::toString(x) + "," + gz::toString(y);
}


float Vector2::dot(Vector2 v2)
{
    Vector2 v1(x,y);

    v1.normalise();
    v2.normalise();

    return acos((v1.x * v2.x) + (v1.y * v2.y)) * RADTODEG;
}

void Vector2::rotate(float angle)
{
    float theta = (angle * DEGTORAD);
    float cs = cos(theta);
    float sn = sin(theta);

    float newx = x * cs - y * sn;
    float newy = x * sn + y * cs;

    set(newx,newy);
}

void Vector2::rotateAround(float angle, Vector2 vec)
{
    rotateAround(angle, vec.x, vec.y);
}

void Vector2::rotateAround(float angle, float origin_x, float origin_y)
{
    float px = x - origin_x;
    float py = y - origin_y;

    float theta = (angle * DEGTORAD);
    float cs = cos(theta);
    float sn = sin(theta);

    float newx = (px * cs - py * sn) + origin_x;
    float newy = (px * sn + py * cs) + origin_y;

    set(newx,newy);
}

void Vector2::setMagnitude(float magnitude)
{
    normalise();
    x *= magnitude;
    y *= magnitude;
}

void Vector2::normalise()
{
    float magnitude = getMagnitude();
    if (magnitude > 0)
    {
        x /= magnitude;
        y /= magnitude;
    }
}

void Vector2::set(float xval, float yval)
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


Vector2 Vector2::operator+(float scalar)
{
    return Vector2(x + scalar, y + scalar);
}

Vector2 Vector2::operator-(float scalar)
{
    return Vector2(x - scalar, y - scalar);
}

Vector2 Vector2::operator*(float scalar)
{
    return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator/(float scalar)
{
    return Vector2(x / scalar, y / scalar);
}

void Vector2::operator+=(float scalar)
{
    x += scalar;
    y += scalar;
}

void Vector2::operator-=(float scalar)
{
    x -= scalar;
    y -= scalar;
}

void Vector2::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
}

void Vector2::operator/=(float scalar)
{
    x /= scalar;
    y /= scalar;
}







