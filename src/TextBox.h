#pragma once
#ifndef __TEXT_BOX__
#define __TEXT_BOX__

#include "Graphic.h"
#include <string>
#include <Windows.h>

/**
*	����ڷκ��� �ؽ�Ʈ�� �Է¹��� �� �ִ� �ؽ�Ʈ ���� Ŭ����.
*	@date	2018.11.28
*	@author	�輺��
*/
class TextBox : public Graphic
{
private:
	sf::RenderWindow textBox; ///<Win32 �ؽ�Ʈ �ڽ�
	bool multiLine; ///<���� �� ���� ����
	unsigned int maxLen; ///<�ִ� ���� ��
	sf::Vector2f position;
	sf::Vector2f size;

	bool textEvent(sf::Uint32 code);
	void updateText();

public:
	TextBox(float x, float y, float width, float height, HWND parent, HINSTANCE instance = NULL, bool multi_line = false);
	virtual ~TextBox() {};
	void draw(sf::RenderWindow* window) override;
	void setText(const std::wstring& str);
	void setFont(sf::Font& font);
	std::wstring getText();
	bool pollEvent(sf::Event e) override;
	bool pollEvent(CustomWinEvent e) override;
	bool hasPoint(const sf::Vector2f& point) override;

	bool loadFontFrom(const std::string& filepath);
	void setBackgroundColor(sf::Color color);
	void setBorderColor(sf::Color color);
	void setBorderSize(float size);
	void setTextColor(sf::Color color);
	void setCharacterSize(unsigned int size);

	sf::Color getBackgroundColor();
	sf::Color getBorderColor();
	float getBorderSize();
	sf::Color getTextColor();
	unsigned int getCharacterSize();

	void setMaxLength(unsigned int len);
	unsigned int getMaxLength();

	void SetPosition(float x, float y) override;

	void SetPositionX(float x) override;

	void SetPositionY(float y) override;

	sf::Vector2f GetPosition() override;

	sf::Vector2f GetSize() override;

};
#endif