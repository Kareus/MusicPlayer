#pragma once
#ifndef __TEXT_LABEL__
#define __TEXT_LABEL__

#include "Graphic.h"
#include <string>
#include <Windows.h>

class TextLabel : public Graphic
{
private:
	sf::Text text;
	sf::Font font;
	std::wstring str;
	sf::Vector2f position;

	void updateText();

public:
	TextLabel(const std::wstring& str);
	virtual ~TextLabel() {};
	void draw(sf::RenderWindow* window) override;
	void setText(const std::wstring& str);
	void setFont(sf::Font& font);
	std::wstring getText();
	bool pollEvent(sf::Event e) override;
	bool pollEvent(CustomWinEvent e) override { return false; }
	bool hasPoint(const sf::Vector2f& point) override;

	void setTextColor(sf::Color color);
	void setCharacterSize(unsigned int size);

	sf::Color getTextColor();
	unsigned int getCharacterSize();

	void SetPosition(float x, float y) override;

	void SetPositionX(float x) override;

	void SetPositionY(float y) override;

	sf::Vector2f GetPosition() override;

	sf::Vector2f GetSize() override;

};
#endif