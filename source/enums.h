#ifndef ENUMS_H_INCLUDED
#define ENUMS_H_INCLUDED

enum Event
{
    Clicked,
    Toggled_on,
    Toggled_off,
};

enum DrawMode
{
    CURVE,
    HANDLES,
    DEBUG,
};

enum ButtonGroup
{
    TOOLS,
    DRAWMODES,
};

enum Buttons
{
    BTN_ADD,
    BTN_DELETE,
    BTN_MOVE,
    BTN_EXPORT,
    BTN_OPTIONS,
};

enum State
{
    NORMAL,
    HOVER,
    ACTIVE,
};

enum Align
{
    TOP,
    LEFT,
    MIDDLE,
    BOTTOM,
    RIGHT
};

enum ExtraKeys
{
    KEY_MOUSE_LEFT          = 101,
    KEY_MOUSE_RIGHT         = 102,
    KEY_MOUSE_MIDDLE        = 103,
    KEY_MOUSE_BUTTON_4      = 104,
    KEY_MOUSE_BUTTON_5      = 105,
    KEY_MOUSE_SCROLL_UP     = 106,
    KEY_MOUSE_SCROLL_DOWN   = 107,
};

#endif // ENUMS_H_INCLUDED
