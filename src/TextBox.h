#pragma once
#ifndef __TEXT_BOX__
#define __TEXT_BOX__

#include "Graphic.h"
#include <string>

class TextBox : public Graphic
{
private:
	sf::RectangleShape shape;
	sf::Text text;
	sf::Font font;
	std::wstring str;
	bool focus;
	int cursorPos;
	int maxLen;
	float x;
	float y;
	float width;
	float height;

public:
	TextBox(float x, float y, float width, float height);
	void draw(sf::RenderWindow* window) override;
	void setText(const std::wstring& str);
	void setFont(sf::Font& font);
	std::wstring getText();
	bool pollEvent(sf::Event e);
	void setFocus(bool f);

};
#endif