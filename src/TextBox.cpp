#include "TextBox.h"

TextBox::TextBox(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	
	shape = sf::RectangleShape(sf::Vector2f(width, height));
	shape.setFillColor(sf::Color::White);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(0.1f);
	font = sf::Font();
	str = L"";
	text = sf::Text(str, font);
	focus = false;
	cursorPos = 0;
	maxLen = -1;
}

void TextBox::draw(sf::RenderWindow* window)
{
	window->draw(shape);
	window->draw(text);
}

void TextBox::setText(const std::wstring& str)
{
	this->str = str;
	text.setString(str);
}

std::wstring TextBox::getText()
{
	return str;
}

void TextBox::setFont(sf::Font& font)
{
	this->font = font;
	text.setFont(font);
}

void TextBox::setFocus(bool f)
{
	focus = f;
}