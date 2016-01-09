#ifndef BUTTON_H
#define BUTTON_H

#include "general.h"
#include "consts.h"

class Button{
    public:
        Button();
        virtual ~Button();

        ///////////////////////////////////////////////////////
        // VARIABLES
        ///////////////////////////////////////////////////////
        sf::RenderWindow*   window = nullptr;

        // button data
        int     id          = -1;
        int     state       = 0; // Normal, Hover, Active
        int     group       = -1;
        int     h_align     = MIDDLE;
        int     v_align     = MIDDLE;
        bool    isClicked   = false;
        bool    isPressed   = false;
        bool    isRadio     = false;

        // button text
        sf::Text    text;
        Vector2     offset;

        // visuals
        sf::RectangleShape  rec;
        sf::Texture         texture;
        sf::Sprite          img;
        // background color
        sf::Color   c_active    = sf::Color(30,145,235);
        sf::Color   c_hover     = sf::Color(210,206,200);
        sf::Color   c_normal    = sf::Color(220,216,210);
        // outline color
        sf::Color   co_active   = sf::Color(0,0,0);
        sf::Color   co_hover    = sf::Color(128,128,128);
        sf::Color   co_normal   = sf::Color(192,192,192);
        // image color
        sf::Color   ci_active   = sf::Color(255,255,255);
        sf::Color   ci_hover    = sf::Color(255,255,255);
        sf::Color   ci_normal   = sf::Color(255,255,255);


        ///////////////////////////////////////////////////////
        // FUNCTIONS
        ///////////////////////////////////////////////////////
        void handleEvents(Vector2 mouse);
        void render();

        bool isTouching(Vector2 point);
        void loadImage(string file);
        void setPosition(int x, int y);
        void setSize(int w, int h);

        void setBackgroundColors(sf::Color ca, sf::Color ch, sf::Color cn);
        void setOutlineColors(sf::Color ca, sf::Color ch, sf::Color cn);

    private:
        void handleButton(Vector2 mouse);
        void handleRadioButton(Vector2 mouse);
};

#endif // BUTTON_H
