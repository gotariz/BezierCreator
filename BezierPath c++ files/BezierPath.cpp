#include "BezierPath.h"

//////////////////////////////////////////////////////////////////
// VECTOR2
//////////////////////////////////////////////////////////////////
float bz::Vector2::getMagnitude()
{
    return sqrt( (x * x) + (y * y) );
}
//////////////////////////////////////////////////////////////////
void bz::Vector2::normalise()
{
    float magnitude = getMagnitude();
    if (magnitude > 0)
    {
        x /= magnitude;
        y /= magnitude;
    }
}
///////////////////////////////////////////////////////////////////////////
// operators
///////////////////////////////////////////////////////////////////////////
void bz::Vector2::operator=(Vector2 vec)
{
    x = vec.x;
    y = vec.y;
}
//////////////////////////////////////////////////////////////////
bz::Vector2 bz::Vector2::operator+(bz::Vector2 vec)
{
    return bz::Vector2( x + vec.x, y + vec.y );
}
//////////////////////////////////////////////////////////////////
bz::Vector2 bz::Vector2::operator-(bz::Vector2 vec)
{
    return bz::Vector2( x - vec.x, y - vec.y );
}
//////////////////////////////////////////////////////////////////
void bz::Vector2::operator+=(bz::Vector2 vec)
{
    x += vec.x;
    y += vec.y;
}
//////////////////////////////////////////////////////////////////
bz::Vector2 bz::Vector2::operator*(float scalar)
{
    return bz::Vector2(x * scalar, y * scalar);
}
//////////////////////////////////////////////////////////////////
// END VECTOR2
//////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////
// BPOINT
//////////////////////////////////////////////////////////////////
void bz::BPoint::setPosition(Vector2 pos)
{
    bz::Vector2 h1Delta = h1 - p;
    bz::Vector2 h2Delta = h2 - p;
    p = pos;
    h1 = h1Delta + pos;
    h2 = h2Delta + pos;
}

void bz::BPoint::setH1Linked(float x, float y)
{
    setH1Linked(Vector2(x,y));
}

void bz::BPoint::setH2Linked(float x, float y)
{
    setH2Linked(Vector2(x,y));
}

void bz::BPoint::setH1Linked(Vector2 pos)
{
    h1 = pos;
    Vector2 delta = pos - p;
    h2 = (delta * -1) + p;
}

void bz::BPoint::setH2Linked(Vector2 pos)
{
    h2 = pos;
    Vector2 delta = pos - p;
    h1 = (delta * -1) + p;
}
//////////////////////////////////////////////////////////////////
// END BPOINT
//////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////
// BEZIERPATH
//////////////////////////////////////////////////////////////////
float bz::BezierPath::normalise(float number)
{
    if (number >= 2) number -= static_cast<int>(number) - 1;
    if (number > 1)  --number;

    return number;
}
//////////////////////////////////////////////////////////////////
bz::Vector2 bz::BezierPath::getPoint(float t)
{
    float  t_range = 1.0 / getCurveCount();
    int    curve_index = static_cast<int>(t / t_range);
    curve_index = std::min(curve_index,getCurveCount()-1);
    float  new_t = (t * getCurveCount()) - curve_index;

    return getPoint(new_t,curve_index);
}
//////////////////////////////////////////////////////////////////
bz::Vector2 bz::BezierPath::getPoint(float t, int n)
{
    if (n >= points.size() || n < 0) return Vector2(0,0);

    float u = 1.f-t;
    float tt = t*t;
    float uu = u*u;
    float ttt = tt*t;
    float uuu = uu*u;

    bz::Vector2 p1 = points.at(n).p;
    bz::Vector2 p2 = points.at(n).h2;
    bz::Vector2 p3 = points.at(n+1).h1;
    bz::Vector2 p4 = points.at(n+1).p;

    bz::Vector2 point = p1 * uuu;  // start point
    point += p2 * t * uu * 3;   // control point 1
    point += p3 * tt * u * 3;   // control point 2
    point += p4 * ttt;          // end point

    return point;
}
//////////////////////////////////////////////////////////////////
int bz::BezierPath::getCurveCount()
{
    return points.size()-1;
}
//////////////////////////////////////////////////////////////////
bz::Vector2 bz::BezierPath::findPoint(Vector2 pos, float distance)
{
    for (int i = points.size() -1; i >= 0; --i)
    {
        bz::Vector2 d0 = pos - points.at(i).p;
        bz::Vector2 d1 = pos - points.at(i).h1;
        bz::Vector2 d2 = pos - points.at(i).h2;

        if      (d2.getMagnitude() < distance) return Vector2(i,2);
        else if (d1.getMagnitude() < distance) return Vector2(i,1);
        else if (d0.getMagnitude() < distance) return Vector2(i,0);
    }

    return bz::Vector2(-1,0);
    return bz::Vector2(-1,0);
}
//////////////////////////////////////////////////////////////////
bz::Vector2 bz::BezierPath::findPoint(float x, float y, float distance)
{
   return findPoint(Vector2(x,y),distance);
}
//////////////////////////////////////////////////////////////////
bool bz::BezierPath::loadPath(std::string path)
{
    std::ifstream file;
    file.open(path.c_str());

    if (!file.is_open()) return false;

    int point = 0;
    std::string line;
    bz::BPoint newPoint(Vector2(0,0));
    while(getline(file,line))
    {
        std::vector<std::string> values = splitString(line,'#');
        if (values.size() < 3) {
            points.clear();
            return false;
        }

        std::string val1 = values[0];
        std::string val2 = values[1];
        std::string val3 = values[2];

        // h1 /////////////////////////////////////////////////////////////
        val1.erase(std::remove(val1.begin(), val1.end(), '['), val1.end());
        val1.erase(std::remove(val1.begin(), val1.end(), ']'), val1.end());
        std::vector<std::string> coords1 = splitString(val1,',');
        if (coords1.size() < 2) {
            points.clear();
            return false;
        }
        Vector2 h1(atof(coords1[0].c_str()),atof(coords1[1].c_str()));

        // p //////////////////////////////////////////////////////////////
        val2.erase(std::remove(val2.begin(), val2.end(), '['), val2.end());
        val2.erase(std::remove(val2.begin(), val2.end(), ']'), val2.end());
        std::vector<std::string> coords2 = splitString(val2,',');
        if (coords2.size() < 2) {
            points.clear();
            return false;
        }
        Vector2 p(atof(coords2[0].c_str()),atof(coords2[1].c_str()));

        // h2 /////////////////////////////////////////////////////////////
        val3.erase(std::remove(val3.begin(), val3.end(), '['), val3.end());
        val3.erase(std::remove(val3.begin(), val3.end(), ']'), val3.end());
        std::vector<std::string> coords3 = splitString(val3,',');
        if (coords3.size() < 2) {
            points.clear();
            return false;
        }
        Vector2 h2(atof(coords3[0].c_str()),atof(coords3[1].c_str()));

        points.push_back( bz::BPoint(p,h1,h2) );

    }
    file.close();
    return true;
}

std::vector<std::string> bz::BezierPath::splitString(std::string path, char delimeter)
{
    std::stringstream ss(path);
    std::vector<std::string> result;

    while( ss.good() )
    {
        std::string str;
        getline( ss, str, delimeter );
        result.push_back(str);
    }

    return result;
}

//////////////////////////////////////////////////////////////////
// END BEZIERPATH
//////////////////////////////////////////////////////////////////






















