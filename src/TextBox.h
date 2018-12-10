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
	bool onlyNumber; ///<숫자만 입력받을 지 여부
	float x; ///<x 좌표
	float y; ///<y 좌표
	float width; ///<너비
	float height; ///<높이
	bool multiLine; ///<여러 줄 가능 여부
	bool input; ///<IME 입력 여부
	bool mouseOver; ///<마우스 오버 여부

	/**
	*	@brief	텍스트 관련 이벤트를 실행하는 함수
	*	@param	code	입력받은 키코드 값
	*/
	bool textEvent(sf::Uint32 code);

	/*
	*	텍스트를 업데이트하는 함수
	*/
	void updateText();

public:

	/**
	*	@brief	텍스트 박스를 생성하는 생성자
	*	@param	x	x좌표
	*	@param	y	y좌표
	*	@param	width	너비
	*	@param	height	폭
	*	@param	multi_line	여러 줄 허용 여부
	*/
	TextBox(float x, float y, float width, float height, bool multi_line = false);

	//기본 소멸자
	virtual ~TextBox() {};

	/**
	*	@brief	윈도우에 그래픽을 렌더링하는 함수
	*	@param	window	렌더링할 윈도우
	*/
	void draw(sf::RenderWindow* window) override;

	/**
	*	@brief	텍스트를 설정하는 함수
	*	@param	str	설정할 텍스트
	*/
	void setText(const std::wstring& str);

	/**
	*	@brief	폰트를 설정하는 함수
	*	@param	font	설정할 폰트
	*/
	void setFont(sf::Font& font);

	/**
	*	@brief	텍스트를 반환하는 함수
	*	@return	입력된 텍스트
	*/
	std::wstring getText();

	/**
	*	@brief	이벤트를 받아 각 그래픽에서 실행하는 함수
	*	@param	e	이벤트
	*	@return	이벤트를 성공적으로 처리했는지 여부가 반환된다
	*/
	bool pollEvent(CustomWinEvent e) override;

	/**
	*	@brief	입력받은 점이 현재 그래픽의 영역에 포함되는지 여부를 반환한다.
	*	@param	point	좌표
	*	@return	포함되면 true, 아니면 false를 반환한다.
	*/
	bool hasPoint(const sf::Vector2f& point) override;

	/**
	*	@brief	폰트를 반환하는 함수
	*	@return	설정된 폰트
	*/
	sf::Font& getFont();

	/**
	*	@brief	입력받은 경로로부터 폰트를 로드하는 함수
	*	@param	filepath	폰트 경로
	*/
	bool loadFontFrom(const std::string& filepath);

	/**
	*	@brief	배경 색깔을 설정하는 함수
	*	@param	color	설정할 배경 색깔
	*/
	void setBackgroundColor(sf::Color color);

	/**
	*	@brief	테두리 색깔을 설정하는 함수
	*	@param	color	설정할 테두리 색깔
	*/
	void setBorderColor(sf::Color color);

	/**
	*	@brief	테두리 크기를 설정하는 함수
	*	@param	size	설정할 테두리 크기
	*/
	void setBorderSize(float size);

	/**
	*	@brief	텍스트 색깔을 설정하는 함수
	*	@param	color	설정할 텍스트 색깔
	*/
	void setTextColor(sf::Color color);

	/**
	*	@brief	글자 크기를 설정하는 함수
	*	@param	size	설정할 글자 크기
	*/
	void setCharacterSize(unsigned int size);

	/**
	*	@brief	배경 색깔을 반환한다.
	*	@return	배경 색깔
	*/
	sf::Color getBackgroundColor();

	/**
	*	@brief	테두리 색깔을 반환한다.
	*	@return	테두리 색깔
	*/
	sf::Color getBorderColor();

	/**
	*	@brief	테두리 크기를 반환한다.
	*	@return	테두리 크기
	*/
	float getBorderSize();

	/**
	*	@brief	글자 색깔을 반환한다.
	*	@return	글자 색깔
	*/
	sf::Color getTextColor();

	/**
	*	@brief	글자 크기를 반환한다.
	*	@return	글자 크기
	*/
	unsigned int getCharacterSize();

	/**
	*	@brief	텍스트 커서 위치를 반환한다.
	*	@return	텍스트 커서 위치
	*/
	float getCursorPosX();

	/**
	*	@brief	최대 글자 수를 설정한다.
	*	@param	len	설정할 최대 글자 수
	*/
	void setMaxLength(unsigned int len);

	/**
	*	@brief	최대 글자 수를 반환한다.
	*	@return	최대 글자 수
	*/
	unsigned int getMaxLength();

	/**
	*	@brief	숫자만 입력받을지 설정한다.
	*	@param	only	숫자만 입력받을지 여부
	*/
	void setOnlyNumber(bool only);

	/**
	*	@brief	숫자만 입력받는지 여부를 반환한다.
	*	@return	숫자만 입력받으면 true, 아니면 false를 반환한다.
	*/
	bool IsOnlyNumber();

	/**
	*	@brief	그래픽의 위치를 지정하는 함수
	*	@param	x	x좌표
	*	@param	y	y좌표
	*/
	void SetPosition(float x, float y) override;

	/**
	*	@brief	그래픽의 위치를 지정하는 함수
	*	@param	x	x좌표
	*/
	void SetPositionX(float x) override;

	/**
	*	@brief	그래픽의 위치를 지정하는 함수
	*	@param	y	y좌표
	*/
	void SetPositionY(float y) override;

	/**
	*	@brief	그래픽의 위치를 반환하는 함수
	*	@return	그래픽의 위치.
	*/
	sf::Vector2f GetPosition() override;

	/**
	*	@brief	그래픽의 크기를 반환하는 함수
	*	@return	그래픽의 크기.
	*/
	sf::Vector2f GetSize() override;

};
#endif