#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "general.h"
#include "Global/global.h"
#include "Utils/Vector2.h"

class Textbox
{
    public:
        Textbox();
        Textbox(int x, int y);
        Textbox(Vector2 pos);
        virtual ~Textbox();

        ///////////////////////////////////////////////////////
        // VARIABLES
        ///////////////////////////////////////////////////////
        sf::RenderWindow* window = nullptr;

        int     validation  = 2; // 0=any 1=integer 2=float
        bool    isFocused   = false;

        Cursor      cursor;
        sf::Text    label;
        string      text        = "";
        int         charLimit   = 10;
        int         cursorIndex = 0;
        int         selFirst    = -1;
        int         selLast     = -1;
        int         textIndent  = 4;

        sf::RectangleShape  rec;

        ///////////////////////////////////////////////////////
        // FUNCTIONS
        ///////////////////////////////////////////////////////
        void render();
        void handleEvents();
        void update();
        void setPosition(int x, int y);

        bool isSelection();
    private:

        void deleteCharacter(int index);
        void deleteSelection();

        void selectText(int start, int end);

        void handleBackspace();
        void handleKeyboardInput();
        void handleInputValidationNone();
        void handleInputValidationInteger();
        void handleInputValidationFloat();

        void renderSelectionRectangle();
        void renderCursor();

        void modifyIndex(int &value,int n);
        void changeCursorPosition(int amount, bool selecting);

};

#endif // TEXTBOX_H
