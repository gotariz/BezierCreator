#include <iostream>

#include "general.h"
#include "App.h"
#include "Utils/Utils.h"

using namespace std;

int main()
{
    int flags = sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close;

    sf::ContextSettings s;
    s.antialiasingLevel = 0;

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Bezier Creator version: 1.01",flags,s);
    window.setFramerateLimit(global.fpsLimit);
    global.window = &window;

    App app(window);
    app.init();

    // run the program as long as the window is open
    while (window.isOpen())
    {
        global.timeDelta = app.clock.getDeltaSeconds();
        // check all the window's events that were triggered since the last iteration of the loop
        app.events();
        app.update();
        app.render();
    }

    return 0;
}
