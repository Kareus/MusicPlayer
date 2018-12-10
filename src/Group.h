#pragma once
#ifndef __GROUP__
#define __GROUP__

#include "Graphic.h"
#include "DoublyLinkedList.h"
#include "DoublyIterator.h"

/**
*	@brief	그래픽을 상속받아 여러 그래픽 항목들을 그룹별로 렌더링하는 클래스.
*	@author	김성주
*/
class Group : public Graphic
{
private:
	sf::Vector2f position;
	DoublyLinkedList<Graphic*> drawings;
	Graphic* focus;
	sf::FloatRect viewRect;
	function<int(Graphic* const&, Graphic* const&)> compareGraphics = [](Graphic* const& g1, Graphic* const& g2) {
		return (*g1 > *g2) - (*g1 < *g2); //포인터는 주소값을 비교하므로, 가리키는 객체의 비교 관계를 반환한다.
	};

public:
	Group();

	~Group();

	int AddGraphic(Graphic* graphic, bool inherit = true);

	int DeleteGraphic(Graphic* graphic);

	void MakeEmpty(bool clear = true);

	int GetLength() const;

	void draw(sf::RenderWindow* window) override;

	void SetPosition(float x, float y) override;

	void SetPositionX(float x) override;

	void SetPositionY(float y) override;

	sf::Vector2f GetPosition() override;

	sf::Vector2f GetSize() override;

	bool pollEvent(CustomWinEvent e) override;

	bool hasPoint(const sf::Vector2f& point) override;

	DoublyIterator<Graphic*> GetIterator();

	void SetViewRect(float x, float y, float width, float height);

	sf::FloatRect GetViewRect();
};
#endif