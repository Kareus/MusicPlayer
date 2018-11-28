#include "Sprite.h"

Sprite::Sprite(const std::string& texturePath)
{
	texture = std::make_shared<sf::Texture>();
	bool a = texture->loadFromFile(texturePath);
	sprite = new sf::Sprite(*texture);
	position.x = 0;
	position.y = 0;
	overColor = sf::Color(180, 180, 180);
	normalColor = sf::Color(255, 255, 255);

	mouseOver = false;
	button = false;
	clickFunc = [](Sprite* spr) {};
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
	if (mouseOver)
	{
		if (!button)
		{
			mouseOver = false;
			sprite->setColor(normalColor);
		}
		else
		{
			POINT point;
			GetCursorPos(&point);

			if (!hasPoint(sf::Vector2f(point.x, point.y)))
			{
				mouseOver = false;
				sprite->setColor(normalColor);
			}
		}
	}

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

void Sprite::setClickFunction(std::function<void(Sprite*)>& func)
{
	this->clickFunc = func;
}

bool Sprite::pollEvent(sf::Event e)
{
	switch (e.type)
	{
	case sf::Event::MouseButtonPressed:
		if (e.mouseButton.button == sf::Mouse::Left)
		{
			clickFunc(this);
			if (mouseOver)
			{
				mouseOver = false;
				sprite->setColor(normalColor);
			}

			return true;
		}
		return false;
	}

	return false;
}

bool Sprite::pollEvent(CustomWinEvent e)
{
	switch (e.type)
	{
	case CustomWinEvent::MouseOver:
		if (!button) return false;
		mouseOver = true;
		sprite->setColor(overColor);
		return true;
	}

	return false;
}

void Sprite::SetButton(bool b)
{
	button = b;
}

bool Sprite::isButton() const
{
	return button;
}