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
	sf::Text text; ///<텍스트 그래픽 객체
	sf::Font font; ///<폰트
	std::wstring str; ///<텍스트의 문자열
	sf::Vector2f position; ///<위치
	sf::FloatRect rect; ///<표시 영역
	sf::Vector2f offset; ///<애니메이션용 차이값 좌표
	TextAlign align; ///<정렬
	std::function<void(TextLabel*)> mouseDownFunc; ///<마우스를 눌렀을 때 실행할 함수
	std::function<void(TextLabel*)> mouseUpFunc; ///<마우스를 뗐을 때 실행할 함수

	/*
	*	텍스트를 업데이트하는 함수
	*/
	void updateText();

public:
	/**
	*	@brief	텍스트 라벨을 생성하는 생성자.
	*	@param	str	설정할 텍스트
	*/
	TextLabel(const std::wstring& str);

	/*
	*	기본 소멸자
	*/
	virtual ~TextLabel() {};

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
	*	@brief	정렬을 설정하는 함수
	*	@param	align	설정할 정렬 상태
	*/
	void setAlign(TextAlign align);

	/**
	*	@brief	정렬 상태를 반환하는 함수
	*	@return	정렬 상태
	*/
	TextAlign getAlign();

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
	*	@brief	표시 영역을 설정한다.
	*	@param	x	x좌표
	*	@param	y	y좌표
	*	@param	width	너비
	*	@param	height	폭
	*/
	void setDisplayRect(float x, float y, float width, float height);

	/**
	*	@brief	표시 영역을 반환한다.
	*	@return	표시 영역
	*/
	sf::FloatRect getDisplayRect();

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
	*	@brief	그래픽의 차이값 좌표를 지정하는 함수
	*	@param	x	x좌표
	*/
	void SetOffsetX(float x);

	/**
	*	@brief	그래픽의 차이값 좌표를 지정하는 함수
	*	@param	y	y좌표
	*/
	void SetOffsetY(float y);

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

	/**
	*	@brief	그래픽의 차이값 좌표를 반환하는 함수
	*	@return	그래픽의 차이값 좌표.
	*/
	sf::Vector2f GetOffset();

	/**
	*	@brief	그래픽 위에서 마우스를 눌렀을 때 실행할 함수를 지정한다.
	*	@param	func	실행할 함수
	*/
	void SetMouseDownFunction(const std::function<void(TextLabel*)>& func);

	/**
	*	@brief	그래픽 위에서 마우스를 뗐을 때 실행할 함수를 지정한다.
	*	@param	func	실행할 함수
	*/
	void SetMouseUpFunction(const std::function<void(TextLabel*)>& func);

};
#endif