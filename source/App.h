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

        BezierPath bezier;

        gzClock clock;

        // points currently being used
        int         point = -1;
        int         pointtype = -1;

        // mouse positions when panning
        Vector2 cameraClick;
        Vector2 primalView;
        Vector2 v;

        bool isEditing = false;
        bool isPanning = false;

        sf::Texture indTexture;
        sf::Sprite  indImg;

        sf::CircleShape ball;
        float time = 0;

        int increasing = 1;
        float dtime = 0;

        sf::Text    labelZoom;
        Button      btnSettings;


        // selected buttons
        ExportWindow    ewindow;
        SettingsWindow  swindow;

        // FUNCTIONS
        void init();
        void events();
        void update();
        void render();

        void handleBezier();
        void handleCamera();
        bool handleGui();

        void renderGrid();
        void renderGUI();
        void renderBezierCurve();
        void renderBezierControls();
        void renderBezierDebug();

    private:
        void recalibrateView();
        void renderCursorIndicator();
        void renderZoomIndicator();
};

#endif // APP_H
