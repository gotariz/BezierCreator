#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "general.h"
#include "GUI/Button.h"
#include "GUI/Textbox.h"

struct MiniManager
{
    sf::RenderWindow*   window = nullptr;
    vector<Textbox>     elements;

    Textbox* at(int i){
        if (i >= 0 && i < elements.size()) {
            return &elements.at(i);
        } else {
            return nullptr;
        }
    }

    int create(int x, int y){
        Textbox txtbox(x,y);
        txtbox.window = window;
        elements.push_back(txtbox);
        return elements.size() - 1;
    }

    void events(){
        for (int i=0;i<elements.size();++i)
            elements.at(i).handleEvents();
    }

    void update(){
        for (int i=0;i<elements.size();++i)
            elements.at(i).update();
    }

    void render(){
        for (int i=0;i<elements.size();++i)
            elements.at(i).render();
    }
};

class SettingsWindow
{
    public:
        SettingsWindow();
        virtual ~SettingsWindow();

        sf::RenderWindow window;

        void init();
        void run();

        void events();
        void update();
        void render();

        void openWindow();

        int normalise = 0; // 0=No,1=x,2=y
        Vector2 mouse;

        Button btnSave;
        Button btnCancel;

        MiniManager     txtboxes;

        sf::Text        txt1;
        sf::Text        txt2;
        sf::Text        txt3;
        sf::Text        txt4;
        sf::Text        txt5;
        sf::Text        txt6;
        sf::Text        txt7;
        sf::Text        txt8;
        sf::Text        txt9;
        sf::Text        txt10;

        // screen
        int GridSize = 0;
        int CursorPosition = 0;
        int RelativeZoom = 0;
        int ControlPoints = 0;
        int Segments = 0;

        int ShowBall = 0;
        int BallAxis = 0;
        int BallAnimationTime = 0;

        int DebugDraw = 0;
        int DebugAnimationTime = 0;

        void loadSettings();
        void applySettings();

        void readSettings();
        void saveSettings();
};

#endif // SETTINGSWINDOW_H
