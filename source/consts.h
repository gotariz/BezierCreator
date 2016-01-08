#ifndef CONSTS_H_INCLUDED
#define CONSTS_H_INCLUDED

#include "SFML/graphics.hpp"
#include "SFML/audio.hpp"
#include "SFML/window.hpp"

using namespace std;

static const float         PI       = 3.141592653589793;

static const float         DEGTORAD = PI / 180.0;
static const float         RADTODEG = 180.0 / PI;
static const float         HANDLE_RADIUS = 5.5;
static const float         PAN_SPEED = 500.0; // pixels per second

// colors
static const sf::Color CBLUE(10,170,235);
static const sf::Color CBLUET(10,170,235,64);
static const sf::Color BG_DARK(106,108,113);

static const sf::Color BLACK(0,0,0);
static const sf::Color GRAY32(32,32,32);
static const sf::Color GRAY64(64,64,64);
static const sf::Color GRAY128(128,128,128);
static const sf::Color GRAY192(192,192,192);
static const sf::Color GRAY246(246,246,246);
static const sf::Color WHITE(255,255,255);

#endif // CONSTS_H_INCLUDED
