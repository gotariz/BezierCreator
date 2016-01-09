#include "Textbox.h"

Textbox::Textbox() : Textbox(0,0)
{
}

Textbox::Textbox(int x, int y)
{
    rec.setSize(sf::Vector2f(100,25));
    rec.setPosition(x,y);

    cursor.setSize(1,21);
    cursor.setPosition(x,y);

    rec.setFillColor(WHITE);
    rec.setOutlineThickness(-1);
    rec.setOutlineColor(GRAY192);
    cursor.rec.setFillColor(BLACK);

    label.setFont(global.fntCourier);
    label.setCharacterSize(15);
    label.setColor(BLACK);
}

Textbox::Textbox(Vector2 pos) : Textbox(pos.x,pos.y)
{
}

Textbox::~Textbox()
{
    //dtor
}


void Textbox::setPosition(int x, int y)
{
    rec.setPosition(x,y);
}

void Textbox::render()
{
    window->draw(rec);

    int x = rec.getPosition().x + textIndent;

    renderSelectionRectangle();

    label.setString(text);
    label.setPosition(x,rec.getPosition().y+2);
    window->draw(label);

    renderCursor();
}

void Textbox::handleEvents()
{
    if (global.keys[KEY_MOUSE_LEFT].isKeyPressed)
    {
        gz::Point<int> mousePos = gz::Point<int>(global.mouse.x,global.mouse.y);
        gz::Rect<int>  button = gz::Rect<int>(
                                         rec.getPosition().x,
                                         rec.getPosition().y,
                                         rec.getSize().x,
                                         rec.getSize().y
                                         );

        if (gz::isCollision(mousePos,button)) {
            isFocused = true;
            selectText(0,text.size());
            cursorIndex = text.size();
        } else {
            isFocused = false;
            selFirst = -1;
            selLast = -1;
        }
    }

    // We do not want to handle events if this textbox does not have focus
    if (!isFocused) return;

    // handle text input
    if (global.isTextEntered)
    {
        if (global.unicode == 8) { // replace with constant/enum
            handleBackspace();
        } else {
            deleteSelection();
            handleKeyboardInput();
        }
    }

    // handle moving the text cursor around and making selections
    bool selecting  = global.keys[sf::Keyboard::LShift].isKeyDown || global.keys[sf::Keyboard::RShift].isKeyDown;
    bool lShift = global.keys[sf::Keyboard::LShift].isKeyPressed;
    bool rShift = global.keys[sf::Keyboard::RShift].isKeyPressed;
    if ( (lShift || rShift) && !isSelection() ){
        selectText(cursorIndex,cursorIndex);
    }

    if (global.keys[sf::Keyboard::Left].isKeyPressed) {
        changeCursorPosition(-1,selecting);
    }
    if (global.keys[sf::Keyboard::Right].isKeyPressed){
        changeCursorPosition(1,selecting);
    }
    if (global.keys[sf::Keyboard::Home].isKeyPressed) {
        changeCursorPosition(-text.size(),selecting);
    }
    if (global.keys[sf::Keyboard::End].isKeyPressed){
        changeCursorPosition(text.size(),selecting);
    }
}

void Textbox::update()
{
    cursor.update(global.timeDelta);
}

//////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////
void Textbox::deleteSelection()
{
    int f = min(selFirst,selLast);
    int l = max(selFirst,selLast) - 1;

    while(f <= l)
    {
        text.erase(text.begin() + l);
        --l;
        if (cursorIndex != f)
            modifyIndex(cursorIndex,-1);
    }

    selFirst = -1;
    selLast = -1;
}

void Textbox::deleteCharacter(int index)
{
    if (index >= 0 && index < text.size())
    {
        text.erase(text.begin() + index);
        modifyIndex(cursorIndex,-1);
    }
}

void Textbox::selectText(int first, int last)
{
    bool firstInbounds  = first>=0 && first<=text.size();
    bool lastInbounds   = last>=0 && last<=text.size();

    if (text.size() <= 0) {
        selFirst    = -1;
        selLast     = -1;
    } else if (firstInbounds && lastInbounds) {
        selFirst    = min(first,last);
        selLast     = max(first,last);
    } else if (firstInbounds) {
        selFirst    = first;
        selLast     = text.size();
    } else if (lastInbounds) {
        selFirst    = 0;
        selLast     = last;
    }
}

bool Textbox::isSelection()
{
    return !(selFirst==-1 || selLast==-1 || selFirst==selLast);
}

void Textbox::renderCursor()
{
    int x = rec.getPosition().x + textIndent;

    label.setString(gz::subString(text,0,cursorIndex-1));
    if (cursorIndex > 0) {
        x += label.getLocalBounds().width + 1;
    }
    if (isFocused && cursor.isVisible) {
        cursor.setPosition(x,rec.getPosition().y + 2);
        window->draw(cursor.rec);
    }
}

void Textbox::renderSelectionRectangle()
{
    if (!isSelection()) return;

    int x = rec.getPosition().x + textIndent + 1;

    int f = min(selFirst,selLast) - 1;
    int l = max(selFirst,selLast) - 1;

    label.setString(gz::subString(text,0,f));
    int ws = label.getLocalBounds().width + 1;
    if (f == -1) ws = 0;

    label.setString(gz::subString(text,0,l));
    int we = label.getLocalBounds().width + 1;
    if (l == -1) we = 0;

    int w = we - ws;
    sf::RectangleShape sel(sf::Vector2f(w,cursor.getSize().y));
    sel.setPosition(x+ws,rec.getPosition().y + 2);
    sel.setFillColor(CBLUE);
    window->draw(sel);
}

void Textbox::handleBackspace()
{
    if (isSelection()) {
        deleteSelection();
    } else {
        deleteCharacter(cursorIndex-1);
    }
}

void Textbox::handleKeyboardInput()
{
    if (text.size() >= charLimit) return;

    if (validation == 0) {
        handleInputValidationNone();
    } else if (validation == 1){
        handleInputValidationInteger();
    } else if (validation == 2) {
        handleInputValidationFloat();
    }
}

void Textbox::handleInputValidationNone()
{
    text.insert(text.begin()+cursorIndex, static_cast<char>(global.unicode));
    modifyIndex(cursorIndex,1);
}

void Textbox::handleInputValidationInteger()
{
    if (global.unicode >= 48 && global.unicode <= 57)
    {
        text.insert(text.begin()+cursorIndex, static_cast<char>(global.unicode));
        modifyIndex(cursorIndex,1);
    }
}

void Textbox::handleInputValidationFloat()
{
    if (global.unicode >= 48 && global.unicode <= 57)
    {
        text.insert(text.begin()+cursorIndex, static_cast<char>(global.unicode));
        modifyIndex(cursorIndex,1);
    }
    else if(global.unicode == 46)
    {
        char c = static_cast<char>(global.unicode);
        if ( text.find(c) == string::npos)
        {
            text.insert(text.begin() + cursorIndex, c);
            modifyIndex(cursorIndex,1);
        }
    }
}

void Textbox::modifyIndex(int &value, int n)
{
    value += n;

    if      (value < 0)              value = 0;
    else if (value > text.size())    value = text.size();
}

void Textbox::changeCursorPosition(int amount, bool selecting)
{
    modifyIndex(cursorIndex,amount);

    if (selecting) {
        selLast = cursorIndex;
    } else {
        selFirst = -1;
        selLast  = -1;
    }
}







