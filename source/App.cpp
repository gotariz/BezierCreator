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

    global.bezier = &bezier;
    global.fntRoboto.loadFromFile("media/fonts/Roboto-Light.ttf");
    global.fntOpenSans.loadFromFile("media/fonts/OpenSans-Light.ttf");

    ewindow.init();
    swindow.init();

    view.set(-100,-global.h+100);
    recalibrateView();

    indTexture.loadFromFile("media/images/indicator.png");
    indImg.setTexture(indTexture);

    labelZoom.setFont(global.fntRoboto);
    labelZoom.setCharacterSize(14);
    labelZoom.setColor(WHITE);

    btnSettings.id = BTN_SETTINGS;
    btnSettings.setSize(45,45);
    btnSettings.loadImage("media/images/options.png");
    btnSettings.setBackgroundColors(CBLUE,GRAY192,BLUEGRAY);
    btnSettings.ci_active = sf::Color::Black;
    btnSettings.rec.setOutlineThickness(0);
}
/////////////////////////////////////////////////////////////////////////////
void App::events()
{
    if (!window.hasFocus()) return;

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
        } else if (event.type == sf::Event::MouseWheelScrolled
                   && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {

            if (global.useRelativeZoom) {
                global.zoom *= 1.0 - (event.mouseWheelScroll.delta / 10.f);
                recalibrateView();
            } else {
                float zoomPercentage = (1.f / global.zoom) + (0.1 * event.mouseWheelScroll.delta); // current zoom +- 10%
                zoomPercentage = max(zoomPercentage,0.1f); // prevent zooming in to negative values
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

    if (global.keys[sf::Keyboard::S].isKeyPressed) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ) {
            ewindow.openWindow();
            ewindow.run();
        }
    }

     if (global.keys[sf::Keyboard::BackSpace].isKeyPressed) {
        bezier.points.clear();
    }

    // set global variables for mouse positions
    global.mouse.x = sf::Mouse::getPosition(window).x;
    global.mouse.y = sf::Mouse::getPosition(window).y;
    sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    global.mouseWorld.x = worldPos.x;
    global.mouseWorld.y = worldPos.y;

    global.updateKeys();
    handleCamera();
    if (!handleGui()) {
        handleBezier();
    }
}
/////////////////////////////////////////////////////////////////////////////
void App::update()
{
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

    // find selected point
    dtime += global.timeDelta * increasing;
    if (dtime >= global.debugAnimationDuration) {
        increasing = -1;
        dtime = global.debugAnimationDuration;
    } else if ( dtime <= 0) {
        increasing = 1;
        dtime = 0;
    }
}
/////////////////////////////////////////////////////////////////////////////
void App::render()
{
    window.clear(GRAY246);
    renderGrid();
    renderBezierCurve();
    renderGUI();
    renderBall();
    window.display();
}
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// EVENT HANDLING FUNCTIONS
/////////////////////////////////////////////////////////////////////////////
void App::handleCamera()
{
    if (isEditing) return;

    KeyboardKey mouseLeft = global.keys.at(KEY_MOUSE_LEFT);
    KeyboardKey space = global.keys.at(sf::Keyboard::Space);

    if (space.isKeyPressed) {
        isPanning = true;
    } else if (space.isKeyReleased) {
        isPanning = false;
    }

    if (mouseLeft.isKeyPressed && isPanning) {
        cameraClick = global.mouse;
        primalView = view;
    } else if (mouseLeft.isKeyDown && space.isKeyDown){
        Vector2 delta = cameraClick - global.mouse;
        view.x = primalView.x + (delta.x * global.zoom);
        view.y = primalView.y + (delta.y* global.zoom);
        recalibrateView();
    }
}
/////////////////////////////////////////////////////////////////////////////
bool App::handleGui()
{
    btnSettings.handleEvents(global.mouse);
    if (btnSettings.isClicked) {
        btnSettings.isClicked = false;
        swindow.openWindow();
        swindow.run();
    }

    return btnSettings.isPressed;
}
/////////////////////////////////////////////////////////////////////////////
void App::handleBezier()
{
    if (isPanning) return;

    KeyboardKey mouseLeft = global.keys[KEY_MOUSE_LEFT];
    KeyboardKey mouseRight = global.keys[KEY_MOUSE_RIGHT];
    bool altDown = global.keys[sf::Keyboard::LAlt].isKeyDown || global.keys[sf::Keyboard::RAlt].isKeyDown;

    int hoverpoint = -1;
    int hovertype = -1;
    if (point == -1) {
        Vector2 result = bezier.findPoint(global.mouseWorld,global.getHandleRadiusSize());
        hoverpoint = result.x;
        hovertype = result.y;
    }

    if (mouseLeft.isKeyPressed) {
        if (hoverpoint != -1) {
            point = hoverpoint;
            pointtype = hovertype;
        } else {
            BPoint newPoint = BPoint(global.mouseWorld);
            bezier.points.push_back(newPoint);
            point = bezier.points.size() - 1;
            pointtype = (bezier.points.size() == 1 ? 1 : 2);
        }
        isEditing = true;
    } else if (!mouseLeft.isKeyDown && mouseRight.isKeyPressed && hoverpoint != -1)  {// right click
        if      (hovertype == 0)  bezier.points.erase(bezier.points.begin()+hoverpoint);
        else if (hovertype == 1)  bezier.points.at(hoverpoint).h1.set(bezier.points.at(hoverpoint).p);
        else if (hovertype == 2)  bezier.points.at(hoverpoint).h2.set(bezier.points.at(hoverpoint).p);
    }

    if (mouseLeft.isKeyDown && point != -1) {
        if      (pointtype == 0)            bezier.points.at(point).setPosition(global.mouseWorld);
        else if (pointtype == 1 && altDown) bezier.points.at(point).h1.set(global.mouseWorld);
        else if (pointtype == 1)            bezier.points.at(point).setH1Linked(global.mouseWorld);
        else if (pointtype == 2 && altDown) bezier.points.at(point).h2.set(global.mouseWorld);
        else if (pointtype == 2)            bezier.points.at(point).setH2Linked(global.mouseWorld);
    }

    if (mouseLeft.isKeyReleased) {
        point = -1;
        pointtype = -1;
        isEditing = false;
    }
}
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// RENDERING FUNCTIONS
/////////////////////////////////////////////////////////////////////////////
void App::renderBall()
{
    if (!global.showBall) return;

    float radius = 10.f * global.zoom;
    ball.setRadius(radius);
    ball.setOrigin(radius,radius);
    ball.setFillColor(RED);
    window.draw(ball);
}

void App::renderGUI()
{
    // render GUI to its own view
    sf::View view = window.getView();
    sf::FloatRect visibleArea(0, 0, global.w, global.h);
    window.setView(sf::View(visibleArea));

    btnSettings.render();
    renderZoomIndicator();
    renderCursorIndicator();

    window.setView(view);
}
/////////////////////////////////////////////////////////////////////////////
void App::renderGrid()
{
    sf::Vector2f camera = window.getView().getCenter();
    float view_w = global.w * global.zoom;
    float view_h = global.h * global.zoom;
    float view_x = camera.x - (view_w / 2);
    float view_y = camera.y - (view_h / 2);
    float view_x2 = view_x + view_w;
    float view_y2 = view_y + view_h;

    float xpos = Utils::roundUp(view_x,global.gridSize);
    float ypos = Utils::roundUp(view_y,global.gridSize);

    sf::Color colour = CBLUET;
    while (xpos <= view_x2)
    {
        colour = xpos == 0 ? sf::Color::Red : CBLUET;
        global.drawLine(Vector2(xpos,view_y),Vector2(xpos,view_y2),colour);
        xpos+=global.gridSize;
    }

    while (ypos <= view_y2)
    {
        colour = ypos == 0 ? sf::Color::Green : CBLUET;
        global.drawLine(Vector2(view_x,ypos),Vector2(view_x2,ypos),colour);
        ypos+=global.gridSize;
    }
}
/////////////////////////////////////////////////////////////////////////////
void App::renderBezierCurve()
{
    for (int n=0;n<bezier.getCurveCount();++n)
    {
        Vector2 q1 = bezier.getPoint(0,n);
        Vector2 q2;
        for (int i=1;i<=global.segmentsPerCurve;++i)
        {
            q2 = bezier.getPoint((float)i / global.segmentsPerCurve,n);
            global.drawLine(q1,q2,sf::Color::Black);
            q1 = q2;
        }
    }

    renderBezierControls();
    renderBezierDebug();
}
/////////////////////////////////////////////////////////////////////////////
void App::renderBezierControls()
{
    if (!global.showControlPoints) return;

    sf::CircleShape c;
    c.setRadius(HANDLE_RADIUS * global.zoom);
    c.setOrigin(HANDLE_RADIUS * global.zoom,HANDLE_RADIUS * global.zoom);
    c.setFillColor(CBLUE);

    Vector2 result = bezier.findPoint(global.mouseWorld,HANDLE_RADIUS * global.zoom);

    for (int i=0;i<bezier.points.size();++i)
    {
        Vector2 p   = bezier.points.at(i).p;
        Vector2 h1  = bezier.points.at(i).h1;
        Vector2 h2  = bezier.points.at(i).h2;

        global.drawLine(h1,p,CBLUE);
        global.drawLine(p,h2,CBLUE);

        c.setFillColor(DBLUE);
        c.setPosition(p.x,p.y);
        global.window->draw(c);

        c.setFillColor(CBLUE);
        c.setPosition(h1.x,h1.y);
        global.window->draw(c);

        c.setFillColor(CBLUE);
        c.setPosition(h2.x,h2.y);
        global.window->draw(c);
    }
}
/////////////////////////////////////////////////////////////////////////////
void App::renderBezierDebug()
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



/////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
/////////////////////////////////////////////////////////////////////////////
void App::recalibrateView()
{
    // apply changes to the camera
    sf::View newView(sf::FloatRect(0,0,global.w,global.h));
    newView.move( sf::Vector2f(static_cast<int>(view.x),static_cast<int>(view.y)) );
    newView.zoom(global.zoom);
    window.setView(newView);
}
/////////////////////////////////////////////////////////////////////////////
void App::renderCursorIndicator()
{
    if (!global.showCursorPosition) return;
    if (global.mouse.y < 45) return;

    sf::Text text;
    text.setFont(global.fntRoboto);
    text.setColor(sf::Color::White);
    text.setCharacterSize(13);
    text.setString("x:" + gz::toString(global.mouseWorld.x) + " y:" + gz::toString(global.mouseWorld.y));

    int tw = text.getLocalBounds().width;
    int x = global.mouse.x+1;
    int y = global.mouse.y-40;
    int w = tw+10;
    int h = 19;
    int i = 5;

    // keep the rectangle in bounds of the window
    if (x+tw+10 > global.w) x = global.w-tw-10;
    else if (x < 0)         x = 0;

    sf::RectangleShape rec(sf::Vector2f(w,h));
    rec.setFillColor(BG_DARK);
    rec.setOutlineThickness(1);
    rec.setOutlineColor(GRAY32);
    rec.setPosition(x,y);

    window.draw(rec);

    indImg.setPosition(global.mouse.x,y+h);
    window.draw(indImg);
    text.setPosition(x+i,y);
    window.draw(text);
}
/////////////////////////////////////////////////////////////////////////////
void App::renderZoomIndicator()
{
    int xpos = 65;
    int ypos = 11;
    int w = 110;
    int h = 21;

    sf::RectangleShape tb(sf::Vector2f(w,h));
    tb.setPosition(xpos,ypos);
    tb.setFillColor(BG_DARK);
    tb.setOutlineColor(GRAY32);
    tb.setOutlineThickness(1);
    global.window->draw(tb);

    int labelx = xpos + (w / 2);

    labelZoom.setString(gz::toString((1.f / global.zoom) * 100) + "%");
    labelx -= (labelZoom.getLocalBounds().width / 2);

    labelZoom.setPosition(labelx,ypos+2);
    global.window->draw(labelZoom);
}
/////////////////////////////////////////////////////////////////////////////
