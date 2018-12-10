#pragma once
#ifndef __GRAPHIC__
#define __GRAPHIC__

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "CustomWinEvent.h"
#include <string>

/**
*	@brief	렌더링할 그래픽에 관련된 base class (추상 클래스이다)
*	@author	김성주
*/
class Graphic
{
protected:
	int id; ///<그래픽의 고유한 primary key
	bool focus; ///<그래픽이 현재 포커싱을 받고 있는지 여부
	std::string data; ///< application에서 데이터를 찾기 위한 변수
	bool visible; ///<그래픽을 렌더링할지 여부

public:
	/**
	*	기본 생성자
	*/
	Graphic() : id(0), focus(false), visible(true) {};

	/**
	*	기본 소멸자
	*/
	virtual ~Graphic() {}; //virtual로 선언하여 dynamic binding된 객체도 올바른 소멸자를 호출할 수 있다.

	/**
	*	@brief	윈도우에 렌더링하기 위한 순수 가상함수
	*	@param	window	렌더링할 윈도우
	*/
	virtual void draw(sf::RenderWindow* window) = 0;

	/**
	*	@brief	이벤트를 받아 각 그래픽에서 실행할 순수 가상함수
	*	@param	e	이벤트
	*	@return	이벤트를 성공적으로 처리했는지 여부가 반환된다
	*/
	virtual bool pollEvent(CustomWinEvent e) = 0;

	/**
	*	@brief	입력받은 점이 현재 그래픽의 영역에 포함되는지 여부를 반환하는 순수 가상함수
	*	@param	point	좌표
	*	@return	포함되면 true, 아니면 false를 반환한다.
	*/
	virtual bool hasPoint(const sf::Vector2f& point) = 0;

	/**
	*	@brief	primary key 설정하는 함수
	*	@param	id	primary key
	*/
	void setID(int id) { this->id = id; }

	/**
	*	@brief	primary key를 반환하는 함수
	*	@return	primary key
	*/
	int getID() const { return id; }

	/**
	*	@brief	포커싱을 설정하는 함수
	*	@param	f	포커스 여부
	*/
	void setFocus(bool f) { focus = f; }

	/**
	*	@brief	그래픽의 좌표를 지정하는 함수
	*	@param	x	x좌표
	*	@param	y	y좌표
	*/
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