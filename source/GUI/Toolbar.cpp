#include "Toolbar.h"

Toolbar::Toolbar()
{
    //ctor
}

Toolbar::~Toolbar()
{
    //dtor
}

void Toolbar::init()
{
    txtZoom.setFont(global.fntRoboto);
    txtZoom.setColor(GRAY246);
    txtZoom.setCharacterSize(14);

    rec.setSize(sf::Vector2f(global.w,45));
    rec.setFillColor(sf::Color(35,39,45));
    rec.setPosition(0,0);

    sf::Color ca = sf::Color(30,145,235);
    sf::Color ch = sf::Color(57,60,66);
    sf::Color cn = sf::Color(35,39,45);

    Button* btnAdd = new Button;
    btnAdd->id = BTN_ADD;
    btnAdd->group = TOOLS;
    btnAdd->isRadio = true;
    btnAdd->setSize(45,45);
    btnAdd->loadImage("media/images/add.png");
    btnAdd->state = ACTIVE;
    btnAdd->setBackgroundColors(ca,ch,cn);
    btnAdd->ci_active = sf::Color::Black;
    btnAdd->rec.setOutlineThickness(0);

    Button* btnDelete = new Button;
    btnDelete->id = BTN_DELETE;
    btnDelete->group = TOOLS;
    btnDelete->isRadio = true;
    btnDelete->setSize(45,45);
    btnDelete->loadImage("media/images/delete.png");
    btnDelete->setBackgroundColors(ca,ch,cn);
    btnDelete->ci_active = sf::Color::Black;
    btnDelete->rec.setOutlineThickness(0);

    Button* btnMove = new Button;
    btnMove->id = BTN_MOVE;
    btnMove->group = TOOLS;
    btnMove->isRadio = true;
    btnMove->setSize(45,45);
    btnMove->loadImage("media/images/move.png");
    btnMove->setBackgroundColors(ca,ch,cn);
    btnMove->ci_active = sf::Color::Black;
    btnMove->rec.setOutlineThickness(0);

    Button* btnExport = new Button;
    btnExport->id = BTN_EXPORT;
    btnExport->setSize(45,45);
    btnExport->loadImage("media/images/export.png");
    btnExport->setBackgroundColors(ca,ch,cn);
    btnExport->ci_active = sf::Color::Black;
    btnExport->rec.setOutlineThickness(0);

    Button* btnOptions = new Button;
    btnOptions->id = BTN_OPTIONS;
    btnOptions->setSize(45,45);
    btnOptions->loadImage("media/images/options.png");
    btnOptions->setBackgroundColors(ca,ch,cn);
    btnOptions->ci_active = sf::Color::Black;
    btnOptions->rec.setOutlineThickness(0);
    bo = btnOptions;

    addLeft(btnAdd);
    addLeft(btnDelete);
    addLeft(btnMove);
    addLeft(btnExport);
    buttons.push_back(btnOptions);
}

void Toolbar::handleEvents()
{
    for (int i=0;i<buttons.size();++i)
    {
        Button* btn = buttons.at(i);
        btn->handleEvents(global.mouse);

        if (!btn->isClicked) continue;

        GUIEvent event(btn->id,btn->group);
        events.push_back(event);
        btn->isClicked = false;

        // untoggle related radio buttons
        if (!btn->isRadio) continue;
        for (int j=0;j<buttons.size();++j) {
            Button* btn2 = buttons.at(j);
            if (btn2->id != btn->id && btn2->isRadio && btn2->group == btn->group)
                btn2->state = NORMAL;
        }
    }

    bo->setPosition(global.w - 45,0); // options button
}

void Toolbar::addLeft(Button* btn)
{
    btn->setPosition(nextx,rec.getPosition().y);
    buttons.push_back(btn);
    nextx+=btn->rec.getSize().x;
}

void Toolbar::render()
{
    rec.setSize(sf::Vector2f(global.w,45));
    global.window->draw(rec);

    for (int i=0;i<buttons.size();++i){
        buttons.at(i)->draw();
    }

    renderZoomBox();
}

void Toolbar::update()
{
}


bool Toolbar::poll(GUIEvent& event)
{
    if (events.size() > 0)
    {
        event.id = events.back().id;
        event.group = events.back().group;
        event.event = events.back().event;
        events.pop_back();
        return true;
    }

    return false;
}

void Toolbar::renderZoomBox()
{
    // draw the zoom box + text
    int xpos = global.w - 165;
    int ypos = 11;
    int w = 110;
    int h = 21;

    sf::RectangleShape tb(sf::Vector2f(w,h));
    tb.setPosition(xpos,ypos);
    tb.setFillColor(BG_DARK);
    tb.setOutlineColor(GRAY32);
    tb.setOutlineThickness(1);
    global.window->draw(tb);

    int tw = txtZoom.getLocalBounds().width;
    txtZoom.setString(gz::toString((1.f / global.zoom) * 100) + "%");
    txtZoom.setPosition(xpos+(w/2)-(tw/2),ypos+2);
    global.window->draw(txtZoom);
}

