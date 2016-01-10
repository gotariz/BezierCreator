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

        gzClock             clock;
        BezierPath          bezier;
        sf::RenderWindow&   window;
        ExportWindow        ewindow; // export window
        SettingsWindow      swindow; // settings window

        // index of mouse overed point
        int                 point = -1;
        int                 pointtype = -1;
        bool                isEditing = false;
        bool                isPanning = false;

        Vector2             cameraClick;
        Vector2             primalView;
        Vector2             view;

        sf::CircleShape     ball;
        sf::Texture         indTexture;
        sf::Sprite          indImg;

        float               time = 0;
        float               dtime = 0;
        int                 increasing = 1;

        sf::Text            labelZoom;
        Button              btnSettings;

        // FUNCTIONS
        void init();
        void events();
        void update();
        void render();

        void handleBezier();
        void handleCamera();
        bool handleGui();

        void renderBall();
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
