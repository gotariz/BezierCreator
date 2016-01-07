#ifndef EXPORTWINDOW_H
#define EXPORTWINDOW_H

#include "GUI/Button.h"
#include "Bezier/BezierPath.h"
#include "general.h"

class ExportWindow
{
    public:
        ExportWindow();
        virtual ~ExportWindow();

        sf::RenderWindow window;

        void init();
        void run();

        void events();
        void update();
        void render();

        void openWindow();

        bool save(string path);

        int normalise = 0; // 0=No,1=x,2=y
        Vector2 mouse;

        Button btnExport;
        Button btnCancel;

        Button btnNO;
        Button btnNX;
        Button btnNY;
};

#endif // EXPORTWINDOW_H
