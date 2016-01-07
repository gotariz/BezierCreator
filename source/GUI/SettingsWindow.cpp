#include "SettingsWindow.h"

SettingsWindow::SettingsWindow()
{
    //ctor
}

SettingsWindow::~SettingsWindow()
{
    //dtor
}

void SettingsWindow::init()
{
    txtboxes.window = &window;

    txt1.setFont(global.fntRoboto);
    txt2.setFont(global.fntRoboto);
    txt3.setFont(global.fntRoboto);
    txt4.setFont(global.fntRoboto);
    txt5.setFont(global.fntRoboto);
    txt6.setFont(global.fntRoboto);
    txt7.setFont(global.fntRoboto);
    txt8.setFont(global.fntRoboto);
    txt9.setFont(global.fntRoboto);
    txt10.setFont(global.fntRoboto);

    txt1.setCharacterSize(13);
    txt2.setCharacterSize(13);
    txt3.setCharacterSize(13);
    txt4.setCharacterSize(13);
    txt5.setCharacterSize(13);
    txt6.setCharacterSize(13);
    txt7.setCharacterSize(13);
    txt8.setCharacterSize(13);
    txt9.setCharacterSize(13);
    txt10.setCharacterSize(13);

    txt1.setColor(BLACK);
    txt2.setColor(BLACK);
    txt3.setColor(BLACK);
    txt4.setColor(BLACK);
    txt5.setColor(BLACK);
    txt6.setColor(BLACK);
    txt7.setColor(BLACK);
    txt8.setColor(BLACK);
    txt9.setColor(BLACK);
    txt10.setColor(BLACK);

    txt1.setPosition(20,23);
    txt2.setPosition(20,53);
    txt3.setPosition(20,83);
    txt4.setPosition(20,113);
    txt5.setPosition(20,143);
    txt6.setPosition(20,193);
    txt7.setPosition(20,223);
    txt8.setPosition(20,253);
    txt9.setPosition(20,303);
    txt10.setPosition(20,333);

    txt1.setString("Grid Size");
    txt2.setString("Show Cursor Position Tooltip");
    txt3.setString("Use Relative Zoom");
    txt4.setString("Show Bezier Control Points");
    txt5.setString("Segments Per Curve");
    txt6.setString("Show Ball Animation Sample");
    txt7.setString("Ball Animation Axis (0=x 1=y 2=x&y)");
    txt8.setString("Ball Animation Duration");
    txt9.setString("Show Debug Control Points");
    txt10.setString("Debug Animation Duration");

    btnSave.window = &window;
    btnSave.setSize(100,25);
    btnSave.setPosition(290,385);
    btnSave.text.setString("Save");
    btnSave.offset.set(0,-2);

    btnCancel.window = &window;
    btnCancel.setSize(100,25);
    btnCancel.setPosition(180,385);
    btnCancel.text.setString("Cancel");
    btnCancel.offset.set(0,-2);

    GridSize                = txtboxes.create(290,20);
    CursorPosition          = txtboxes.create(290,50);
    RelativeZoom            = txtboxes.create(290,80);
    ControlPoints           = txtboxes.create(290,110);
    Segments                = txtboxes.create(290,140);
    ShowBall                = txtboxes.create(290,190);
    BallAxis                = txtboxes.create(290,220);
    BallAnimationTime       = txtboxes.create(290,250);
    DebugDraw               = txtboxes.create(290,300);
    DebugAnimationTime      = txtboxes.create(290,330);
}

void SettingsWindow::run()
{
    while (window.isOpen())
    {
        events();
        update();
        render();
    }
}

void SettingsWindow::events()
{
    global.isTextEntered = false;
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
        else if (event.type == sf::Event::TextEntered)
        {
            global.isTextEntered = true;
            global.unicode = event.text.unicode;
        }
    }

    global.updateKeys();

    global.mx = sf::Mouse::getPosition(window).x;
    global.my = sf::Mouse::getPosition(window).y;
    mouse.x = sf::Mouse::getPosition(window).x;
    mouse.y = sf::Mouse::getPosition(window).y;

    btnSave.handleEvents(mouse);
    btnCancel.handleEvents(mouse);
    txtboxes.events();

    if (btnSave.isClicked) {
        btnSave.isClicked = false;
        applySettings();
        window.close();
    }
    if (btnCancel.isClicked) {
        btnCancel.isClicked = false;
        window.close();
    }
}

void SettingsWindow::update()
{
    txtboxes.update();
}

void SettingsWindow::render()
{
    window.clear(GRAY246);

    sf::RectangleShape rec(sf::Vector2f(400,45));
    rec.setPosition(0,375);
    rec.setFillColor(sf::Color::White);
    rec.setOutlineThickness(1);
    rec.setOutlineColor(sf::Color(192,192,192));

    window.draw(rec);

    btnSave.draw();
    btnCancel.draw();
    txtboxes.render();

    window.draw(txt1);
    window.draw(txt2);
    window.draw(txt3);
    window.draw(txt4);
    window.draw(txt5);
    window.draw(txt6);
    window.draw(txt7);
    window.draw(txt8);
    window.draw(txt9);
    window.draw(txt10);

    window.display();
}

void SettingsWindow::openWindow()
{
    window.create(sf::VideoMode(400, 420), "Settings",sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    loadSettings();
}

void SettingsWindow::loadSettings()
{
    readSettings();

    txtboxes.at(GridSize)->text = gz::toString(global.gridSize);
    txtboxes.at(CursorPosition)->text = gz::toString(global.showCursorPosition);
    txtboxes.at(RelativeZoom)->text = gz::toString(global.useRelativeZoom);
    txtboxes.at(ControlPoints)->text = gz::toString(global.showControlPoints);
    txtboxes.at(Segments)->text = gz::toString(global.segmentsPerCurve);
    txtboxes.at(ShowBall)->text = gz::toString(global.showBall);
    txtboxes.at(BallAxis)->text = gz::toString(global.ballAxis);
    txtboxes.at(BallAnimationTime)->text = gz::toString(global.ballAnimationDuration);
    txtboxes.at(DebugDraw)->text = gz::toString(global.drawDebug);
    txtboxes.at(DebugAnimationTime)->text = gz::toString(global.debugAnimationDuration);
}

void SettingsWindow::applySettings()
{
    global.gridSize = gz::toInt(txtboxes.at(GridSize)->text.c_str(),50);
    global.showCursorPosition = gz::toBool(txtboxes.at(CursorPosition)->text.c_str());
    global.useRelativeZoom = gz::toBool(txtboxes.at(RelativeZoom)->text.c_str());
    global.showControlPoints = gz::toBool(txtboxes.at(ControlPoints)->text.c_str());
    global.segmentsPerCurve = gz::toInt(txtboxes.at(Segments)->text.c_str(),100);
    global.showBall = gz::toBool(txtboxes.at(ShowBall)->text.c_str());
    global.ballAxis = gz::toInt(txtboxes.at(BallAxis)->text.c_str(),1);
    global.ballAnimationDuration = atof(txtboxes.at(BallAnimationTime)->text.c_str());
    global.drawDebug = gz::toBool(txtboxes.at(DebugDraw)->text.c_str());
    global.debugAnimationDuration = atof(txtboxes.at(DebugAnimationTime)->text.c_str());

    saveSettings();
}

void SettingsWindow::readSettings()
{
    std::string line;
    std::ifstream file;
    file.open("config.ini");

    if (file.is_open())
    {
        while(getline(file,line))
        {
            std::pair<std::string,std::string> p = gz::getValuePair(line);
            std::string name = p.first;
            std::string value = p.second;

            if (name == "fps_limit") {
                global.fpsLimit = atoi(value.c_str());
            } else if (name == "grid_size") {
                global.gridSize = atoi(value.c_str());
            } else if (name == "show_cursor_pos") {
                global.showCursorPosition = gz::toBool(value.c_str());
            } else if (name == "use_relative_zoom") {
                global.useRelativeZoom = gz::toBool(value.c_str());
            } else if (name == "show_control_points") {
                global.showControlPoints = gz::toBool(value.c_str());
            } else if (name == "segments_per_curve") {
                global.segmentsPerCurve = atoi(value.c_str());
            } else if (name == "show_ball_preview") {
                global.showBall = gz::toBool(value.c_str());
            } else if (name == "ball_axis") {
                global.ballAxis = atoi(value.c_str());
            } else if (name == "ball_anim_dur") {
                global.ballAnimationDuration = atof(value.c_str());
            } else if (name == "draw_debug") {
                global.drawDebug = gz::toBool(value.c_str());
            } else if (name == "debug_anim_dur") {
                global.debugAnimationDuration = atof(value.c_str());
            }
        }
    }
    else
    {
        gz::log_error("Unable to open file: config.ini");
    }
    file.close();
}

void SettingsWindow::saveSettings()
{
    ofstream s ("config.ini");

    if (s.is_open())
    {
        s << "fps_limit="           << global.fpsLimit << endl;
        s << "grid_size="           << global.gridSize << endl;
        s << "show_cursor_pos="     << global.showCursorPosition << endl;
        s << "use_relative_zoom="   << global.useRelativeZoom << endl;
        s << "show_control_points=" << global.showControlPoints << endl;
        s << "segments_per_curve="  << global.segmentsPerCurve << endl;
        s << "show_ball_preview="   << global.showBall << endl;
        s << "ball_axis="           << global.ballAxis << endl;
        s << "ball_anim_dur="       << global.ballAnimationDuration << endl;
        s << "draw_debug="          << global.drawDebug << endl;
        s << "debug_anim_dur="      << global.debugAnimationDuration << endl;

        s.close();
    }
    else
    {
        s.close();
        cout << "Unable to open config.ini" << endl;;
    }
}


