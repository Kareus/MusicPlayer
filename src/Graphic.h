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
	virtual ~Graphic() {}; //virtual�� �����Ͽ� dynamic binding�� ��ü�� �ùٸ� �Ҹ��ڸ� ȣ���� �� �ִ�.
	virtual void draw(sf::RenderWindow* window) = 0;
	virtual bool pollEvent(sf::Event e) = 0;

	bool operator<(const Graphic& g) { return id < g.id; }
	bool operator>(const Graphic& g) { return id > g.id; }
	bool operator==(const Graphic& g) { return id == g.id; }
};
#endif