#include "TextLabel.h"

TextLabel::TextLabel(const std::wstring& str)
{
	font = sf::Font();

	this->str = str;
	text = sf::Text(str, font);
	text.setFillColor(sf::Color::Black);
	position.x = 0;
	position.y = 0;
	rect.x = -1;
	rect.y = 0;
	align = LEFT;

	focus = false;
}

void TextLabel::draw(sf::RenderWindow* window)
{
	if (rect.x < 0)
	{
		window->draw(text);
		return;
	}
	else {
		glEnable(GL_SCISSOR_TEST);
		glScissor(position.x, window->getSize().y - position.y - rect.y, rect.x, rect.y);
		window->draw(text);
		glDisable(GL_SCISSOR_TEST);
	}
}

void TextLabel::setText(const std::wstring& str)
{
	this->str = str;
	text.setString(str);
	updateText();
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

bool TextLabel::pollEvent(CustomWinEvent e)
{
	return false;
}

void TextLabel::updateText()
{
	if (rect.x < 0 || align == LEFT)
	{
		text.setPosition(position);
		return;
	}

	sf::FloatRect bound = text.getLocalBounds();
	switch (align)
	{

	case MIDDLE:
		text.setPosition(position.x + (rect.x - bound.width) / 2, position.y);
		break;

	case RIGHT:
		text.setPosition(position.x + rect.x - bound.width, position.y);
		break;
	}
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

bool TextLabel::loadFontFrom(const std::string& filepath)
{
	return font.loadFromFile(filepath);
}

sf::Font& TextLabel::getFont()
{
	return font;
}

void TextLabel::setDisplayRect(float width, float height)
{
	rect.x = width;
	rect.y = height;

	updateText();
}

sf::Vector2f TextLabel::getDisplayRect()
{
	return rect;
}

void TextLabel::setAlign(TextAlign align)
{
	this->align = align;
}

TextAlign TextLabel::getAlign()
{
	return align;
}