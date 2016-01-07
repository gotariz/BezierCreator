#include "Button.h"

Button::Button()
{
    rec.setOutlineThickness(-1);
    rec.setOutlineColor(co_normal);

    text.setFont(global.fntRoboto);
    text.setColor(sf::Color::Black);
    text.setCharacterSize(13);
    text.setScale(1,1);
}

Button::~Button()
{

}

void Button::loadImage(string file)
{
    if (texture.loadFromFile(file)) {
        img.setTexture(texture);
        img.setPosition(rec.getPosition());
    } else {
        cout << "error loading image:" << file << endl;
    }
}

void Button::handleEvents(Vector2 mouse)
{
    if (isRadio) {
        handleRadioButton(mouse);
    } else {
        handleButton(mouse);
    }
}

void Button::draw()
{
    sf::RenderWindow* w = (!window ? global.window : window);

    // set the colors based on state
    if (state == NORMAL) {
        rec.setFillColor(c_normal);
        rec.setOutlineColor(co_normal);
        img.setColor(ci_normal);
    } else if (state == HOVER) {
        rec.setFillColor(c_hover);
        rec.setOutlineColor(co_hover);
        img.setColor(ci_hover);
    } else if (state == ACTIVE) {
        rec.setFillColor(c_active);
        rec.setOutlineColor(co_active);
        img.setColor(ci_active);
    }

    w->draw(rec);
    w->draw(img);

    // draw the text
    int x = rec.getPosition().x +(rec.getSize().x / 2) + offset.x;
    int y = rec.getPosition().y + (rec.getSize().y / 2) + offset.y;
    int tw = text.getLocalBounds().width;
    int th = text.getCharacterSize();

    if (h_align == MIDDLE)      x -= (tw/2);
    else if (h_align == RIGHT)  x -= (tw);

    if (v_align == MIDDLE)      y-= (th/2);
    else if (v_align == BOTTOM) y-= (th);

    text.setPosition(x,y);
    w->draw(text);
}

void Button::setPosition(int x, int y)
{
    rec.setPosition(x,y);
    img.setPosition(x,y);
}

void Button::setSize(int w, int h)
{
    rec.setSize(sf::Vector2f(w,h));
}

bool Button::isTouching(Vector2 point)
{
    if (point.x < rec.getPosition().x) return false;
    if (point.y < rec.getPosition().y) return false;

    if (point.x > rec.getPosition().x + rec.getSize().x - 1) return false;
    if (point.y > rec.getPosition().y + rec.getSize().y - 1) return false;

    return true;
}

void Button::setBackgroundColors(sf::Color ca, sf::Color ch, sf::Color cn)
{
    c_active = ca;
    c_hover = ch;
    c_normal = cn;
}

void Button::setOutlineColors(sf::Color ca, sf::Color ch, sf::Color cn)
{
    co_active = ca;
    co_hover = ch;
    co_normal = cn;
}

void Button::handleButton(Vector2 mouse)
{
    bool mouseover = isTouching(Vector2(mouse.x,mouse.y));
    bool pressed = global.keys[KEY_MOUSE_LEFT].isKeyPressed;
    bool released = global.keys[KEY_MOUSE_LEFT].isKeyReleased;

    if (mouseover) {
        if (pressed){
            state = ACTIVE;
            isPressed = true;
        } else if (released && isPressed) {
            state = HOVER;
            isClicked = true;
            isPressed = false;
        } else if (isPressed) {
            state = ACTIVE;
        } else if (!isPressed) {
            state = HOVER;
        }
    } else {
        if (released) {
            isPressed = false;
        }
        state = NORMAL;
    }
}

void Button::handleRadioButton(Vector2 mouse)
{
    bool mouseover = isTouching(Vector2(mouse.x,mouse.y));
    bool pressed = global.keys[KEY_MOUSE_LEFT].isKeyPressed;
    bool released = global.keys[KEY_MOUSE_LEFT].isKeyReleased;

    if (state == ACTIVE) return;

    if (mouseover) {

        if (pressed) {
            isPressed = true;
        } else if (released) {
            state = ACTIVE;
            isPressed = false;
            isClicked = true;
        } else {
            state = HOVER;
        }

    } else {
        if (released) {
            isPressed = false;
        }
        state = NORMAL;
    }
}


