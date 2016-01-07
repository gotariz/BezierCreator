#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#include "Utils/Vector2.h"

struct Cursor
{
    sf::RectangleShape  rec;
    bool                isVisible = true;
    float               showTime = 0.5;
    float               hideTime = 0.25;
    float               curTime = 0;

    Cursor(int w = 0, int h = 0)
    {
        rec.setSize(sf::Vector2f(w,h));
    }

    void setSize(int w, int h)
    {
        rec.setSize(sf::Vector2f(w,h));
    }

    void setPosition(int x, int y)
    {
        rec.setPosition(x,y);
    }

    void update(float delta)
    {
        curTime += delta;
        if (isVisible && curTime >= showTime) {
            curTime -= showTime;
            isVisible = false;
        } else if (!isVisible && curTime >= hideTime) {
            curTime -= hideTime;
            isVisible = true;
        }
    }

    Vector2 getSize()
    {
        return Vector2(rec.getSize().x,rec.getSize().y);
    }

    Vector2 getPosition()
    {
        return Vector2(rec.getPosition().x,rec.getPosition().y);
    }
};

struct GUIEvent
{
    int id;
    int group;
    int event;

    GUIEvent(int id = -1, int group = -1, int event = -1)
    {
        this->id = id;
        this->group = group;
        this->event = event;
    }
};

struct KeyboardKey
{
   sf::Keyboard::Key   m_key;
   int                 m_code;
   std::string         m_name;

   bool        isKeyDown = false;
   bool        isKeyPressed = false;
   bool        isKeyReleased = false;

   bool keyRepeat = false;
   double repeatTime = 0.1;
   double elapsedTime = 0;

   void updateKey(bool isDown, double timeDelta)
   {
       isKeyPressed     = (!isKeyDown && isDown);
       isKeyReleased    = (isKeyDown && !isDown);
       isKeyDown        = isDown;

       if (!keyRepeat) return;
       if (isKeyDown)
       {
           elapsedTime += timeDelta;
           if (elapsedTime >= repeatTime)
           {
               isKeyPressed = true;
               elapsedTime = 0;
           }
       }
       if (isKeyReleased)
       {
           elapsedTime = 0;
       }
   }

   KeyboardKey(int code = sf::Keyboard::Unknown, std::string name = "Unknown") : m_code(code), m_name(name)
   {
       if (code >= 0 && code <= sf::Keyboard::KeyCount)
           m_key = static_cast<sf::Keyboard::Key>(code);
       else
           m_key = sf::Keyboard::Unknown;
   }
};

struct Camera{

    double x;
    double y;
    int w;
    int h;

    Camera() : x(0), y(0), w(0), h(0) {}
    Camera(double xpos, double ypos, int width, int height) : x(xpos), y(ypos), w(width), h(height){}

    void setPosition(double xpos, double ypos)
    {
        x = xpos;
        y = ypos;
    }

    void setPosition(Vector2 pos)
    {
        x = pos.x;
        y = pos.y;
    }

    void setSize(int width, int height)
    {
        w = width;
        h = height;
    }

    int getX(){return static_cast<int>(x);}
    int getY(){return static_cast<int>(y);}

};


#endif // STRUCTS_H_INCLUDED
