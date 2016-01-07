#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "Utils/Vector2.h"
#include "general.h"
#include "GUI/Button.h"
#include "GUI/Textbox.h"
#include "Utils/Utils.h"
#include "structs.h"

class Toolbar
{
    public:
        Toolbar();
        virtual ~Toolbar();


        sf::Text txtZoom;
        int nextx = 0;              // next button in the tool bar will be at this position
        sf::RectangleShape rec;     // the toolbar rectangle
        vector<Button*> buttons;
        vector<GUIEvent> events;

        void init();
        void handleEvents();
        void update();
        void render();
        void addLeft(Button* btn);
        bool poll(GUIEvent& event);
        void renderZoomBox();

        Button* bo = nullptr;
};

#endif // TOOLBAR_H
