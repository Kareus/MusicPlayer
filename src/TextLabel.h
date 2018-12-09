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
	sf::FloatRect rect;
	sf::Vector2f offset;
	TextAlign align;
	std::function<void(TextLabel*)> mouseDownFunc; ///<���콺�� ������ �� ������ �Լ�
	std::function<void(TextLabel*)> mouseUpFunc; ///<���콺�� ���� �� ������ �Լ�

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

	void setDisplayRect(float x, float y, float width, float height);
	sf::FloatRect getDisplayRect();

	void SetPosition(float x, float y) override;

	void SetPositionX(float x) override;

	void SetPositionY(float y) override;

	void SetOffsetX(float x);

	void SetOffsetY(float y);

	sf::Vector2f GetPosition() override;

	sf::Vector2f GetSize() override;

	sf::Vector2f GetOffset();

	void SetMouseDownFunction(const std::function<void(TextLabel*)>& func);

	void SetMouseUpFunction(const std::function<void(TextLabel*)>& func);

};
#endif