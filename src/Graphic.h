#pragma once
#ifndef __GRAPHIC__
#define __GRAPHIC__

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "CustomWinEvent.h"
#include <string>

class Graphic
{
protected:
	int id;
	bool focus;
	std::string data; ///< application에서 데이터를 찾기 위한 변수
	bool visible;

public:
	Graphic() : id(0), focus(false), visible(true) {};
	virtual ~Graphic() {}; //virtual로 선언하여 dynamic binding된 객체도 올바른 소멸자를 호출할 수 있다.
	virtual void draw(sf::RenderWindow* window) = 0;
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
	void Show() { visible = true; }
	void Hide() { visible = false; }
	bool isVisible() { return visible; }

	bool operator<(const Graphic& g) const { return id < g.id; }
	bool operator>(const Graphic& g) const { return id > g.id; }
	bool operator==(const Graphic& g) const { return id == g.id; }

	void SetData(const std::string& data) { this->data = data; }
	std::string GetData() { return data; }
};
#endif