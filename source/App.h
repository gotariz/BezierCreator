#ifndef APP_H
#define APP_H

#include "general.h"
#include "Utils/Vector2.h"
#include "Bezier/BezierPath.h"
#include "Utils/gzClock.h"
#include "GUI/Toolbar.h"
#include "GUI/ExportWindow.h"
#include "GUI/SettingsWindow.h"
#include "Utils/Utils.h"

class App
{
    public:
        App(sf::RenderWindow& rwindow);
        virtual ~App();

        sf::RenderWindow& window;

        gzClock clock;
        Toolbar         toolbar;

        // points currently being used
        int         point = -1;
        int         pointtype = -1;

        // mouse positions when panning
        Vector2 cameraClick;
        Vector2 primalView;

        BezierPath bezier;

        float vx = 0;
        float vy = 0;

        sf::Texture indTexture;
        sf::Sprite  indImg;

        sf::CircleShape ball;
        float time = 0;

        int increasing = 1;
        float dtime = 0;


        // selected buttons
        int activeTool = BTN_ADD;
        ExportWindow    ewindow;
        SettingsWindow  swindow;

        // FUNCTIONS
        void init();

        void handleEvents();
        void handleCameraEvents();
        void handleGUIEvents();

        void update();

        void render();
        void renderGrid();
        void renderGUI();
        void renderBezier();
        void renderBezierControls();
        void renderDebugBezierControls();

        void recalibrateView();

    private:

        void renderCursorPosition();

        void addPoint();
        void deletePoint();
        void movePoint();
};

#endif // APP_H
