#include "TextLabel.h"

TextLabel::TextLabel(const std::wstring& str)
{
	font = sf::Font();

	this->str = str;
	text = sf::Text(str, font);
	text.setFillColor(sf::Color::Black);
	position.x = 0;
	position.y = 0;

	focus = false;
}

void TextLabel::draw(sf::RenderWindow* window)
{
	window->draw(text);
}

void TextLabel::setText(const std::wstring& str)
{
	this->str = str;
	text.setString(str);
}

std::wstring TextLabel::getText()
{
	return str;
}

void TextLabel::setFont(sf::Font& font)
{
	this->font = font;
	text.setFont(font);
}

bool TextLabel::pollEvent(sf::Event e)
{
	return false;
}

void TextLabel::updateText()
{
	text.setPosition(position);
}

bool TextLabel::hasPoint(const sf::Vector2f& point)
{
	sf::FloatRect rect = text.getLocalBounds();
	return position.x <= point.x && point.x < position.x + rect.width && position.y <= point.y && point.y < position.y + rect.height;
}

void TextLabel::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	updateText();
}

void TextLabel::SetPositionX(float x)
{
	position.x = x;
	updateText();
}

void TextLabel::SetPositionY(float y)
{
	position.y = y;
	updateText();
}

sf::Vector2f TextLabel::GetPosition()
{
	return position;
}

sf::Vector2f TextLabel::GetSize()
{
	sf::FloatRect rect = text.getLocalBounds();
	return sf::Vector2f(rect.width, rect.height);
}

void TextLabel::setTextColor(sf::Color color)
{
	text.setFillColor(color);
}

sf::Color TextLabel::getTextColor()
{
	return text.getFillColor();
}

void TextLabel::setCharacterSize(unsigned int size)
{
	text.setCharacterSize(size);
}

unsigned int TextLabel::getCharacterSize()
{
	return text.getCharacterSize();
}