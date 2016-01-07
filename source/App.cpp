#include "App.h"

App::App(sf::RenderWindow& rwindow) : window(rwindow)
{
    //ctor
}

App::~App()
{
    //dtor
}

void App::init()
{
    swindow.readSettings();

    global.fntRoboto.loadFromFile("media/fonts/Roboto-Light.ttf");
    global.fntCourier.loadFromFile("media/fonts/OpenSans-Light.ttf");
    // change the camera's x and y position so the world origin is in the bottom left
    vx = -100;
    vy = -global.h + 100;
    recalibrateView();

    toolbar.init();
    ewindow.init();
    swindow.init();

    global.bezier = &bezier;

    // indicator for mouse position
    indTexture.loadFromFile("media/images/indicator.png");
    indImg.setTexture(indTexture);

    ball.setRadius(10.f);
    ball.setOrigin(10,10);
    ball.setFillColor(sf::Color::Red);
}

void App::handleEvents()
{
    global.isTextEntered = false;
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::Resized) {
            global.w = event.size.width;
            global.h = event.size.height;
            recalibrateView();
        } else if (event.type == sf::Event::MouseWheelScrolled) {

            if (event.mouseWheelScroll.wheel != sf::Mouse::VerticalWheel) continue;

            if (global.useRelativeZoom) {
                global.zoom *= 1.0 - (event.mouseWheelScroll.delta / 10.f);
                recalibrateView();
            } else {
                double zoomPercentage = (1.f / global.zoom) + (0.1 * event.mouseWheelScroll.delta); // current zoom +- 10%
                zoomPercentage = max(zoomPercentage,0.1); // prevent zooming in to negative values
                global.zoom = 1.f / zoomPercentage;
                recalibrateView();
            }
        }
        else if (event.type == sf::Event::TextEntered)
        {
            global.isTextEntered = true;
            global.unicode = event.text.unicode;
        }
    }

    // set global variables for mouse positions
    global.mx = sf::Mouse::getPosition(window).x;
    global.my = sf::Mouse::getPosition(window).y;
    sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    global.wx = worldPos.x;
    global.wy = worldPos.y;
    if (global.zoom == 1) { // when at 100% zoom ignore decimals due to rounding errors
        global.wx = Utils::roundNearestf(worldPos.x,1);
        global.wy = Utils::roundNearestf(worldPos.y,1);
    }

    global.updateKeys();
    handleCameraEvents();
    handleGUIEvents();
}

void App::handleCameraEvents()
{
    // moving the camera with the mouse while holding down space
    if (global.keys.at(KEY_MOUSE_LEFT).isKeyPressed && global.keys.at(sf::Keyboard::Space).isKeyDown)
    {
        cameraClick = Vector2(global.mx,global.my);
        primalView.set(vx,vy);
    } else if (global.keys.at(KEY_MOUSE_LEFT).isKeyDown && global.keys.at(sf::Keyboard::Space).isKeyDown){
        Vector2 delta = cameraClick - Vector2(global.mx,global.my);
        vx = primalView.x + (delta.x * global.zoom);
        vy = primalView.y + (delta.y* global.zoom);
        recalibrateView();
    }


    // pan the camera left, right, up and down
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        vx -= (PAN_SPEED * global.timeDelta);
        recalibrateView();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        vx += (PAN_SPEED * global.timeDelta);
        recalibrateView();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
        vy += (PAN_SPEED * global.timeDelta);
        recalibrateView();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
        vy -= (PAN_SPEED * global.timeDelta);
        recalibrateView();
    }
}

void App::recalibrateView()
{
    // apply changes to the camera
    sf::View view(sf::FloatRect(0,0,global.w,global.h));
    view.move( sf::Vector2f(static_cast<int>(vx),static_cast<int>(vy)) );
    view.zoom(global.zoom);
    window.setView(view);
}


void App::handleGUIEvents()
{
    toolbar.handleEvents();

    GUIEvent e;
    while (toolbar.poll(e))
    {
        if (e.group == TOOLS) {
            activeTool = e.id;
        } else {
            if (e.id == BTN_EXPORT) {
                ewindow.openWindow();
                ewindow.run();
            } else if (e.id == BTN_OPTIONS) {
                swindow.openWindow();
                swindow.run();
            }
        }
    }

    // ignore GUI events while space is held down
    if (global.my < 45 || global.keys[sf::Keyboard::Space].isKeyDown) return;

    if (activeTool == BTN_ADD) {
        addPoint();
    } else if (activeTool == BTN_DELETE) {
        deletePoint();
    } else if (activeTool == BTN_MOVE) {
        movePoint();
    }
}

void App::update()
{
    toolbar.update();

    // update the preview ball animation
    if (bezier.points.size() > 1) {
        time += global.timeDelta;
        if (time > global.ballAnimationDuration) time = 0;

        Vector2 pos = bezier.getPoint(time / global.ballAnimationDuration);
        if (global.ballAxis == 0) { // x
            ball.setPosition(pos.x,0);
        } else if (global.ballAxis == 1){
            ball.setPosition(0,pos.y);
        } else if (global.ballAxis == 2){
            ball.setPosition(pos.x,pos.y);
        }
    }


    dtime += global.timeDelta * increasing;
    if (dtime >= global.debugAnimationDuration) {
        increasing = -1;
        dtime = global.debugAnimationDuration;
    } else if ( dtime <= 0) {
        increasing = 1;
        dtime = 0;
    }
}

void App::render()
{
    window.clear(GRAY246);
    renderGrid();
    renderBezier();
    renderGUI();
    if (global.showBall) {
            window.draw(ball);
    }
    window.display();
}

void App::renderGUI()
{
    // render GUI to its own view
    sf::View view = window.getView();
    sf::FloatRect visibleArea(0, 0, global.w, global.h);
    window.setView(sf::View(visibleArea));

    toolbar.render();
    renderCursorPosition();

    window.setView(view);
}

void App::renderGrid()
{
    // draws the grid lines
    for (int x = -10000; x < 10000; x+=global.gridSize){ // vertical lines
        global.drawLine(Vector2(x,-10000),Vector2(x,10000),CBLUET);
    }

    for (int y = -10000; y < 10000; y+=global.gridSize){ // horizontal lines
        global.drawLine(Vector2(-10000,y),Vector2(10000,y),CBLUET);
    }

    // draw the center lines
    global.drawLine(Vector2(-10000,0),Vector2(10000,0),CBLUE);
    global.drawLine(Vector2(0,-10000),Vector2(0,10000),CBLUE);
}

void App::renderBezier()
{
    for (int n=0;n<bezier.getCurveCount();++n)
    {
        Vector2 q1 = bezier.getPoint(0,n);
        Vector2 q2;
        for (int i=1;i<=global.segmentsPerCurve;++i)
        {
            q2 = bezier.getPoint((double)i / global.segmentsPerCurve,n);
            global.drawLine(q1,q2,sf::Color::Black);
            q1 = q2;
        }
    }

    renderBezierControls();
    renderDebugBezierControls();
}

void App::renderBezierControls()
{
    if (!global.showControlPoints) return;

    sf::CircleShape c;
    c.setRadius(HANDLE_RADIUS * global.zoom);
    c.setOrigin(HANDLE_RADIUS * global.zoom,HANDLE_RADIUS * global.zoom);
    c.setFillColor(CBLUE);

    for (int i=0;i<bezier.points.size();++i)
    {
        Vector2 p   = bezier.points.at(i).p;
        Vector2 h1  = bezier.points.at(i).h1;
        Vector2 h2  = bezier.points.at(i).h2;

        global.drawLine(h1,p,CBLUE);
        global.drawLine(p,h2,CBLUE);

        c.setPosition(p.x,p.y);
        global.window->draw(c);

        c.setPosition(h1.x,h1.y);
        global.window->draw(c);

        c.setPosition(h2.x,h2.y);
        global.window->draw(c);
    }
}

void App::renderDebugBezierControls()
{
    if (!global.drawDebug) return;

    float t = dtime / global.debugAnimationDuration;

    sf::CircleShape c;
    c.setRadius(HANDLE_RADIUS * global.zoom);
    c.setOrigin(HANDLE_RADIUS * global.zoom,HANDLE_RADIUS * global.zoom);
    c.setFillColor(CBLUE);

   if (bezier.points.size() < 2) return; // nothing to draw

    for (int i=0;i<bezier.points.size()-1;++i)
    {
        BPoint p1 = bezier.points.at(i);
        BPoint p2 = bezier.points.at(i+1);
        global.drawLine(p1.p,p1.h2,CBLUE);
        global.drawLine(p1.h2,p2.h1,CBLUE);
        global.drawLine(p2.h1,p2.p,CBLUE);
        c.setPosition(p1.p.x,p1.p.y);window.draw(c);
        c.setPosition(p1.h2.x,p1.h2.y);window.draw(c);
        c.setPosition(p2.h1.x,p2.h1.y);window.draw(c);
        c.setPosition(p2.p.x,p2.p.y);window.draw(c);

        Vector2 p3 = Utils::lerp(t,p1.p,p1.h2);
        Vector2 p4 = Utils::lerp(t,p1.h2,p2.h1);
        Vector2 p5 = Utils::lerp(t,p2.h1,p2.p);
        global.drawLine(p3,p4,CBLUE);
        global.drawLine(p4,p5,CBLUE);
        c.setPosition(p3.x,p3.y);window.draw(c);
        c.setPosition(p4.x,p4.y);window.draw(c);
        c.setPosition(p5.x,p5.y);window.draw(c);

        Vector2 p6 = Utils::lerp(t,p3,p4);
        Vector2 p7 = Utils::lerp(t,p4,p5);
        global.drawLine(p6,p7,CBLUE);
        c.setPosition(p6.x,p6.y);window.draw(c);
        c.setPosition(p7.x,p7.y);window.draw(c);

        Vector2 p8 = Utils::lerp(t,p6,p7);
        c.setPosition(p8.x,p8.y);window.draw(c);
    }
}

/////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
/////////////////////////////////////////////////////////////////////////////

void App::renderCursorPosition()
{
    if (!global.showCursorPosition) return;
    if (global.my < 45) return;

    sf::Text text;
    text.setFont(global.fntRoboto);
    text.setColor(sf::Color::White);
    text.setCharacterSize(13);
    text.setString("x:" + gz::toString(global.wx) + " y:" + gz::toString(global.wy));

    int tw = text.getLocalBounds().width;
    int x = global.mx+1;
    int y = global.my-40;
    int w = tw+10;
    int h = 19;
    int i = 5;  // indent

    // keep the rectangle in bounds of the window
    if (x+tw+10 > global.w) x = global.w-tw-10;
    else if (x < 0)         x = 0;

    sf::RectangleShape rec(sf::Vector2f(w,h));
    rec.setFillColor(BG_DARK);
    rec.setOutlineThickness(1);
    rec.setOutlineColor(GRAY32);
    rec.setPosition(x,y);

    window.draw(rec);

    indImg.setPosition(global.mx,y+h);
    window.draw(indImg);
    text.setPosition(x+i,y);
    window.draw(text);
}

void App::addPoint()
{
    if (global.keys[KEY_MOUSE_LEFT].isKeyPressed)
    {
        BPoint newPoint = BPoint( Vector2(global.wx,global.wy) );
        bezier.points.push_back(newPoint);
        point = bezier.points.size() - 1;
    }
    else if (global.keys[KEY_MOUSE_LEFT].isKeyDown && point >= 0)
    {
        if (bezier.points.size() == 1){
            bezier.points.at(point).setH1Linked(Vector2(global.wx,global.wy));
        } else {
            bezier.points.at(point).setH2Linked(Vector2(global.wx,global.wy));
        }
    }
    else if (global.keys[KEY_MOUSE_LEFT].isKeyReleased)
    {
        point = -1;
    }
}

void App::deletePoint()
{
    if (global.keys[KEY_MOUSE_LEFT].isKeyPressed)
    {
        Vector2 result = bezier.findPoint(Vector2(global.wx,global.wy),(HANDLE_RADIUS*global.zoom));
        point = result.x;
        pointtype = result.y;

        if (point < 0) return;

        if (pointtype == 0) {
            bezier.points.erase(bezier.points.begin()+point);
        } else if (pointtype == 1) {
            bezier.points.at(point).h1.set(bezier.points.at(point).p);
        } else if (pointtype == 2) {
            bezier.points.at(point).h2.set(bezier.points.at(point).p);
        }
    }
}

void App::movePoint()
{
    if (global.keys[KEY_MOUSE_LEFT].isKeyPressed)
    {
        Vector2 result = bezier.findPoint(Vector2(global.wx,global.wy),(global.getHandleRadiusSize()));
        point = result.x;
        pointtype = result.y;
    }
    else if (global.keys[KEY_MOUSE_LEFT].isKeyDown)
    {
        if (point < 0) return;

        bool altDown = global.keys[sf::Keyboard::LAlt].isKeyDown || global.keys[sf::Keyboard::RAlt].isKeyDown;
        BPoint& p = bezier.points.at(point);

        // move the corresponding point
        if (pointtype == 0) {
                bezier.points.at(point).setPosition(Vector2(global.wx,global.wy));
        } else if (pointtype == 1) {
            if (altDown)    p.h1.set(global.wx,global.wy);
            else            p.setH1Linked(global.wx,global.wy);
        } else if (pointtype == 2) {
            if (altDown)    p.h2.set(global.wx,global.wy);
            else            p.setH2Linked(global.wx,global.wy);
        }
    }
    else if (global.keys[KEY_MOUSE_LEFT].isKeyReleased)
    {
        point = -1;
        pointtype = -1;
    }
}










































