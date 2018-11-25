#include "GraphicElement.h"

GraphicElement::GraphicElement(const std::string& texturePath)
{
	texture = std::make_shared<sf::Texture>();
	texture->loadFromFile(texturePath);
	sprite = new sf::Sprite(*texture);
	position.x = 0;
	position.y = 0;
}

GraphicElement::GraphicElement(const GraphicElement& data)
{
	if (sprite != nullptr)
	{
		delete sprite;
		texture.reset();

		sprite = nullptr;
		texture = nullptr;
	} //sprite가 있을 경우 메모리 해제.

	texture = data.texture; //shared_ptr은 자동으로 복사 할당됨.
	sprite = new sf::Sprite(*texture);
	position = data.position;
}

GraphicElement::~GraphicElement()
{
	delete sprite;
	texture.reset();

	sprite = nullptr;
	texture = nullptr;
}

void GraphicElement::updatePosition()
{
	sprite->setPosition(position);
}

sf::Sprite* GraphicElement::GetSprite() const
{
	return sprite;
}

void GraphicElement::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	updatePosition();
}

void GraphicElement::SetPositionX(float x)
{
	position.x = x;
	updatePosition();
}

void GraphicElement::SetPositionY(float y)
{
	position.y = y;
	updatePosition();
}

sf::Vector2f GraphicElement::GetPosition()
{
	return position;
}