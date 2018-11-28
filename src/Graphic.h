#pragma once
#ifndef __GRAPHIC__
#define __GRAPHIC__

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "CustomWinEvent.h"

class Graphic
{
protected:
	int id;
	bool focus;

public:
	Graphic() : id(0), focus(false) {};
	virtual ~Graphic() {}; //virtual로 선언하여 dynamic binding된 객체도 올바른 소멸자를 호출할 수 있다.
	virtual void draw(sf::RenderWindow* window) = 0;
	virtual bool pollEvent(sf::Event e) = 0;
	virtual bool pollEvent(CustomWinEvent e) = 0;
	virtual bool hasPoint(const sf::Vector2f& point) = 0;
	void setID(int id) { this->id = id; }
	int getID() const { return id; }
	void setFocus(bool f) { focus = f; }

	virtual void SetPosition(float x, float y) = 0;
	virtual void SetPositionX(float x) = 0;
	virtual void SetPositionY(float y) = 0;
	virtual sf::Vector2f GetPosition() = 0;
	virtual sf::Vector2f GetSize() = 0;

	bool operator<(const Graphic& g) { return id < g.id; }
	bool operator>(const Graphic& g) { return id > g.id; }
	bool operator==(const Graphic& g) { return id == g.id; }
};
#endif