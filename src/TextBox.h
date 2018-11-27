#pragma once
#ifndef __TEXT_BOX__
#define __TEXT_BOX__

#include "Graphic.h"
#include <string>
#include <Windows.h>

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
	bool multiLine;

	bool textEvent(sf::Uint32 code);

public:
	TextBox(float x, float y, float width, float height, bool multi_line = false);
	virtual ~TextBox() {};
	void draw(sf::RenderWindow* window) override;
	void setText(const std::wstring& str);
	void setFont(sf::Font& font);
	std::wstring getText();
	bool pollEvent(sf::Event e) override;
	void setFocus(bool f);

};
#endif