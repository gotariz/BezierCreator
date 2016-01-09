#ifndef GLOBAL_H
#define GLOBAL_H

#include "structs.h"
#include "consts.h"
#include "Utils/Vector2.h"
#include "enums.h"

class BezierPath;

class Global
{
    public:
        Global();
        virtual ~Global();

        // fonts
        sf::Font    fntRoboto;
        sf::Font    fntCourier;

        Camera              camera;
        BezierPath*         bezier = nullptr;
        sf::RenderWindow*   window = nullptr;

        int     w = 1280;
        int     h = 720;
        float   zoom = 1.f;
        float   timeDelta = 0;

        Vector2 mouse;
        Vector2 mouseWorld;
        //int mx = 0;
        //int my = 0;
        //float wx = 0;
        //float wy = 0;

        // events
        vector<KeyboardKey>	keys;
        void updateKeys();
		bool isTextEntered = false;
		int unicode = 0;

        void drawLine(int x, int y, int x2, int y2, sf::Color c);
        void drawLine(Vector2 p1, Vector2 p2, sf::Color c);
        bool isTouching(Vector2 point, sf::RectangleShape box);
        bool isTouching(Vector2 point, int x, int y, int w, int h);

        float getHandleRadiusSize();

        // settings variables
        int     fpsLimit = 60;
        int     gridSize = 50;
        bool    showCursorPosition = true;
        bool    useRelativeZoom = false;
        bool    showControlPoints = true;
        int     segmentsPerCurve = 100;
        bool    showBall = true;
        int     ballAxis = 0;
        float   ballAnimationDuration = 1.f;
        bool    drawDebug = false;
        float   debugAnimationDuration = 1.f;
};

extern Global global;

#endif // GLOBAL_H
