#pragma once
#ifndef __GROUP__
#define __GROUP__

#include "Graphic.h"
#include "DoublyLinkedList.h"
#include "DoublyIterator.h"

/**
*	@brief	그래픽을 상속받아 여러 그래픽 항목들을 그룹별로 렌더링하는 클래스.
*	@author	Kareus
*/
class Group : public Graphic
{
private:
	sf::Vector2f position; ///<그래픽의 위치
	DoublyLinkedList<Graphic*> drawings; ///<그룹에서 렌더링할 리스트
	Graphic* focus; ///<포커스로 설정된 그래픽
	sf::FloatRect viewRect; ///<렌더링할 구역
	function<int(Graphic* const&, Graphic* const&)> compareGraphics = [](Graphic* const& g1, Graphic* const& g2) {
		return (*g1 > *g2) - (*g1 < *g2); //포인터는 주소값을 비교하므로, 가리키는 객체의 비교 관계를 반환한다.
	};

public:

	/*
	*	기본 생성자
	*/
	Group();

	/*
	*	기본 소멸자
	*/
	~Group();

	/**
	*	@brief	그래픽을 그룹에 추가하는 함수
	*	@param	graphic	추가할 그래픽
	*	@param	inherit	그룹의 data를 추가할 그래픽이 상속받을지 여부
	*	@return	추가에 성공하면 1, 아니면 0을 반환한다.
	*/
	int AddGraphic(Graphic* graphic, bool inherit = true);

	/**
	*	@brief	그래픽을 그룹에서 삭제하는 함수
	*	@param	graphic	삭제할 그래픽
	*	@return	삭제에 성공하면 1, 아니면 0을 반환한다.
	*/
	int DeleteGraphic(Graphic* graphic);

	/**
	*	@brief	그룹의 그래픽 리스트를 비우는 함수
	*	@param	clear	각 그래픽의 메모리를 해제할지 여부
	*/
	void MakeEmpty(bool clear = true);

	/**
	*	@brief	그룹의 그래픽 개수를 반환하는 함수
	*	@return	그래픽 개수.
	*/
	int GetLength() const;

	/**
	*	@brief	윈도우에 그래픽을 렌더링하는 함수
	*	@param	window	렌더링할 윈도우
	*/
	void draw(sf::RenderWindow* window) override;

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
	*	@brief	그래픽의 렌더링 리스트의 반복자를 생성하여 반환하는 함수
	*	@return	렌더링 리스트의 반복자
	*/
	DoublyIterator<Graphic*> GetIterator();

	/**
	*	@brief	그룹의 표시 영역을 지정하는 함수
	*	@param	x	x좌표
	*	@param	y	y좌표
	*	@param	width	너비
	*	@param	height	폭
	*/
	void SetViewRect(float x, float y, float width, float height);

	/**
	*	@brief	그래픽의 표시 영역을 반환하는 함수
	*	@return	그래픽의 표시 영역
	*/
	sf::FloatRect GetViewRect();
};
#endif