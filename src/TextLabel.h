#pragma once
#ifndef __TEXT_LABEL__
#define __TEXT_LABEL__

#include "Graphic.h"
#include <string>
#include <functional>
#include <Windows.h>

enum TextAlign
{
	LEFT, MIDDLE, RIGHT
};

class TextLabel : public Graphic
{
private:
	sf::Text text;
	sf::Font font;
	std::wstring str;
	sf::Vector2f position;
	sf::Vector2f rect;
	TextAlign align;
	std::function<void(TextLabel*)> mouseDownFunc; ///<마우스를 눌렀을 때 실행할 함수
	std::function<void(TextLabel*)> mouseUpFunc; ///<마우스를 뗐을 때 실행할 함수

	void updateText();

public:
	TextLabel(const std::wstring& str);
	virtual ~TextLabel() {};
	void draw(sf::RenderWindow* window) override;
	void setText(const std::wstring& str);
	void setFont(sf::Font& font);
	void setAlign(TextAlign align);
	TextAlign getAlign();
	std::wstring getText();
	bool pollEvent(CustomWinEvent e) override;
	bool hasPoint(const sf::Vector2f& point) override;
	sf::Font& getFont();

	bool loadFontFrom(const std::string& filepath);
	void setTextColor(sf::Color color);
	void setCharacterSize(unsigned int size);

	sf::Color getTextColor();
	unsigned int getCharacterSize();

	void setDisplayRect(float width, float height);
	sf::Vector2f getDisplayRect();

	void SetPosition(float x, float y) override;

	void SetPositionX(float x) override;

	void SetPositionY(float y) override;

	sf::Vector2f GetPosition() override;

	sf::Vector2f GetSize() override;

	void SetMouseDownFunction(const std::function<void(TextLabel*)>& func);

	void SetMouseUpFunction(const std::function<void(TextLabel*)>& func);

};
#endif