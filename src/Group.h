#pragma once
#ifndef __GROUP__
#define __GROUP__

#include "Graphic.h"
#include "DoublyLinkedList.h"
#include "DoublyIterator.h"

class Group : public Graphic
{
private:
	sf::Vector2f position;
	DoublyLinkedList<Graphic*> drawings;
	Graphic* focus;

public:
	Group();

	~Group();

	int AddGraphic(Graphic* graphic);

	int DeleteGraphic(Graphic* graphic);

	void MakeEmpty();

	int GetLength() const;

	void draw(sf::RenderWindow* window) override;

	void SetPosition(float x, float y) override;

	void SetPositionX(float x) override;

	void SetPositionY(float y) override;

	sf::Vector2f GetPosition() override;

	sf::Vector2f GetSize() override;

	bool pollEvent(sf::Event e) override;

	bool pollEvent(CustomWinEvent e) override;

	bool hasPoint(const sf::Vector2f& point) override;

	DoublyIterator<Graphic*> GetIterator();
};
#endif