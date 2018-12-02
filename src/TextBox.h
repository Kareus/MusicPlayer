#pragma once
#ifndef __TEXT_BOX__
#define __TEXT_BOX__

#include "Graphic.h"
#include <string>
#include <Windows.h>

/**
*	사용자로부터 텍스트를 입력받을 수 있는 텍스트 상자 클래스.
*	@date	2018.11.28
*	@author	김성주
*/
class TextBox : public Graphic
{
private:
	sf::RectangleShape shape; ///<배경 사각형
	sf::RectangleShape cursor; ///<텍스트 커서
	sf::Clock timer; ///<커서 깜빡임을 위한 타이머
	sf::Text text; ///<텍스트
	sf::Font font; ///<폰트
	std::wstring str; ///<텍스트용 문자열
	std::wstring origin; ///<IME 입력에 쓸 원본 문자열
	std::wstring selection; ///<선택 문자열
	static const unsigned int BLINK = 500; ///<깜빡임용 시간 상수
	bool blink; ///<깜빡임 여부
	int cursorPos; ///<현재 커서 위치
	unsigned int maxLen; ///<최대 길이 (무제한은 -1)
	float x; ///<x 좌표
	float y; ///<y 좌표
	float width; ///<너비
	float height; ///<높이
	bool multiLine; ///<여러 줄 가능 여부
	bool input; ///<IME 입력 여부
	bool mouseOver; ///<마우스 오버 여부

	bool textEvent(sf::Uint32 code);
	void updateText();

public:
	TextBox(float x, float y, float width, float height, bool multi_line = false);
	virtual ~TextBox() {};
	void draw(sf::RenderWindow* window) override;
	void setText(const std::wstring& str);
	void setFont(sf::Font& font);
	std::wstring getText();
	bool pollEvent(sf::Event e) override;
	bool pollEvent(CustomWinEvent e) override;
	bool hasPoint(const sf::Vector2f& point) override;
	sf::Font& getFont();

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

	float getCursorPosX();

	void setMaxLength(unsigned int len);
	unsigned int getMaxLength();

	void SetPosition(float x, float y) override;

	void SetPositionX(float x) override;

	void SetPositionY(float y) override;

	sf::Vector2f GetPosition() override;

	sf::Vector2f GetSize() override;

};
#endif