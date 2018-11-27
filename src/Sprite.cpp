#include "Sprite.h"

Sprite::Sprite(const std::string& texturePath)
{
	texture = std::make_shared<sf::Texture>();
	texture->loadFromFile(texturePath);
	sprite = new sf::Sprite(*texture);
	position.x = 0;
	position.y = 0;
}

Sprite::Sprite(const Sprite& data)
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

Sprite::~Sprite()
{
	delete sprite;
	texture.reset();

	sprite = nullptr;
	texture = nullptr;
}

void Sprite::updatePosition()
{
	sprite->setPosition(position);
}

void Sprite::draw(sf::RenderWindow* window)
{
	window->draw(*sprite);
}

void Sprite::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	updatePosition();
}

void Sprite::SetPositionX(float x)
{
	position.x = x;
	updatePosition();
}

void Sprite::SetPositionY(float y)
{
	position.y = y;
	updatePosition();
}

sf::Vector2f Sprite::GetPosition()
{
	return position;
}

sf::Vector2f Sprite::GetSize()
{
	sf::FloatRect rect = sprite->getLocalBounds();
	return sf::Vector2f(rect.width, rect.height);
}

bool Sprite::hasPoint(const sf::Vector2f& point)
{
	sf::FloatRect rect = sprite->getLocalBounds();
	return position.x <= point.x && point.x < position.x + rect.width && position.y <= point.y && point.y < position.y + rect.height;
}