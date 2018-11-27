#pragma once
#ifndef __GRAPHIC__
#define __GRAPHIC__

#include <SFML/Graphics.hpp>

class Graphic
{
protected:
	int id;

public:
	Graphic() : id(0) {};
	virtual void draw(sf::RenderWindow* window) = 0;

	bool operator<(const Graphic& g) { return id < g.id; }
	bool operator>(const Graphic& g) { return id > g.id; }
	bool operator==(const Graphic& g) { return id == g.id; }
};
#endif