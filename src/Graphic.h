#pragma once
#ifndef __GRAPHIC__
#define __GRAPHIC__

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

class Graphic
{
protected:
	int id;

public:
	Graphic() : id(0) {};
	virtual ~Graphic() {}; //virtual로 선언하여 dynamic binding된 객체도 올바른 소멸자를 호출할 수 있다.
	virtual void draw(sf::RenderWindow* window) = 0;
	virtual bool pollEvent(sf::Event e) = 0;

	bool operator<(const Graphic& g) { return id < g.id; }
	bool operator>(const Graphic& g) { return id > g.id; }
	bool operator==(const Graphic& g) { return id == g.id; }
};
#endif