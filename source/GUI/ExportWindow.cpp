#include "ExportWindow.h"

ExportWindow::ExportWindow()
{
    //ctor
}

ExportWindow::~ExportWindow()
{
    //dtor
}

void ExportWindow::openWindow()
{
    window.create(sf::VideoMode(230, 150), "Save File",sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
}

void ExportWindow::init()
{
    btnExport.window = &window;
    btnExport.setSize(100,25);
    btnExport.setPosition(10,115);
    btnExport.text.setString("Save");
    btnExport.offset.set(0,-2);

    btnCancel.window = &window;
    btnCancel.setSize(100,25);
    btnCancel.setPosition(120,115);
    btnCancel.text.setString("Cancel");
    btnCancel.offset.set(0,-2);

    btnNO.window = &window;
    btnNO.text.setString("Don't Normalise");
    btnNO.isRadio = true;
    btnNO.setSize(15,15);
    btnNO.setPosition(20,20);
    btnNO.c_normal = sf::Color(192,192,192);
    btnNO.c_hover = GRAY64;
    btnNO.loadImage("media/images/radio.png");
    btnNO.state = ACTIVE;
    btnNO.h_align = LEFT;
    btnNO.offset.set(25,-3);

    btnNX.window = &window;
    btnNX.text.setString("Normalise X");
    btnNX.isRadio = true;
    btnNX.setSize(15,15);
    btnNX.setPosition(20,45);
    btnNX.c_normal = sf::Color(192,192,192);
    btnNX.c_hover = GRAY64;
    btnNX.loadImage("media/images/radio.png");
    btnNX.h_align = LEFT;
    btnNX.offset.set(25,-3);

    btnNY.window = &window;
    btnNY.text.setString("Normalise Y");
    btnNY.isRadio = true;
    btnNY.setSize(15,15);
    btnNY.setPosition(20,70);
    btnNY.c_normal = sf::Color(192,192,192);
    btnNY.c_hover = GRAY64;
    btnNY.loadImage("media/images/radio.png");
    btnNY.h_align = LEFT;
    btnNY.offset.set(25,-3);
}

void ExportWindow::run()
{
    while (window.isOpen())
    {
        events();
        update();
        render();
    }
}

void ExportWindow::events()
{
    sf::Event event;
    while (window.pollEvent(event)){
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    global.updateKeys();

    mouse.x = sf::Mouse::getPosition(window).x;
    mouse.y = sf::Mouse::getPosition(window).y;

    btnExport.handleEvents(mouse);
    btnCancel.handleEvents(mouse);
    btnNO.handleEvents(mouse);
    btnNX.handleEvents(mouse);
    btnNY.handleEvents(mouse);

    if (btnNO.isClicked) {
        btnNX.state = NORMAL;
        btnNY.state = NORMAL;
        btnNO.isClicked = false;
        normalise = 0;
    } else if (btnNX.isClicked) {
        btnNO.state = NORMAL;
        btnNY.state = NORMAL;
        btnNX.isClicked = false;
        normalise = 1;
    } else if (btnNY.isClicked) {
        btnNO.state = NORMAL;
        btnNX.state = NORMAL;
        btnNY.isClicked = false;
        normalise = 2;
    }

    if (btnExport.isClicked) {

        btnExport.isClicked = false;

        char const* a = {"*.xml"};
        const char* const* b = &a;

        char const* result = tinyfd_saveFileDialog("Save File","",1,b,"*.xml");
        if (result != NULL) {
            string file = result;
            save(file + ".xml");
            window.close();
        }
    }

    if (btnCancel.isClicked) {
        btnCancel.isClicked = false;
        window.close();
    }
}

void ExportWindow::update()
{

}

void ExportWindow::render()
{
    window.clear(GRAY246);

    sf::RectangleShape rec(sf::Vector2f(400,45));
    rec.setPosition(0,105);
    rec.setFillColor(sf::Color::White);
    rec.setOutlineThickness(1);
    rec.setOutlineColor(sf::Color(192,192,192));

    window.draw(rec);

    btnNO.render();
    btnNX.render();
    btnNY.render();

    btnExport.render();
    btnCancel.render();
    window.display();
}

bool ExportWindow::save(string path)
{
    BezierPath& bezier = *global.bezier;

    Vector2 s;
    Vector2 e;
    if (bezier.points.size() > 0) {
        s = bezier.points.at(0).p;
        e = bezier.points.back().p;
    }

    float w = abs(e.x - s.x);
    float h = abs(e.y - s.y);

    float divisor = 0;

    if      (normalise == 0)    divisor = 1; // do not normalise
    else if (normalise == 1)    divisor = w; // normalise x
    else if (normalise == 2)    divisor = h; // normalise y

    if      (divisor == 0)      divisor = 1; // don't allow division by 0

    for (int i=0;i<bezier.points.size();++i)
    {
        Vector2 p = bezier.points.at(i).p;
        Vector2 h1 = bezier.points.at(i).h1;
        Vector2 h2 = bezier.points.at(i).h2;

        bezier.points.at(i).h1.x   = (h1.x - s.x) / divisor;
        bezier.points.at(i).p.x    = (p.x - s.x) / divisor;
        bezier.points.at(i).h2.x   = (h2.x - s.x) / divisor;

        bezier.points.at(i).h1.y   = (h1.y - s.y) / divisor;
        bezier.points.at(i).p.y    = (p.y - s.y) / divisor;
        bezier.points.at(i).h2.y   = (h2.y - s.y) / divisor;
    }

    // TODO: write to XML
    XMLDocument doc;
    XMLNode* root = doc.NewElement("root");
    doc.InsertFirstChild(root);

    for (int i=0;i<bezier.points.size();++i)
    {
        XMLElement* element = doc.NewElement("point");
        element->SetAttribute("h1",bezier.points.at(i).h1.toString().c_str());
        element->SetAttribute("p",bezier.points.at(i).p.toString().c_str());
        element->SetAttribute("h2",bezier.points.at(i).h2.toString().c_str());
        root->InsertEndChild(element);
    }

    XMLError result = doc.SaveFile(path.c_str());
    if (result != XML_SUCCESS)
    {
        tinyfd_messageBox(
                    "Error",
                    "There was an error saving the file: " + result,
                    "ok",
                    "error",
                    1 );

        return false;
    }

    return true;
}





























