#include "TextLabel.h"

TextLabel::TextLabel(const std::wstring& str)
{
	font = sf::Font();

	this->str = str;
	text = sf::Text(str, font);
	text.setFillColor(sf::Color::Black);
	position.x = 0;
	position.y = 0;
	rect.left = -1;
	rect.top = 0;
	rect.width = 0;
	rect.height = 0;
	offset.x = 0;
	offset.y = 0;
	align = LEFT;

	focus = false;
	mouseDownFunc = [](TextLabel*) {return; };
	mouseUpFunc = [](TextLabel*) {return; };
}

void TextLabel::draw(sf::RenderWindow* window)
{
	if (rect.left < 0)
	{
		window->draw(text);
		return;
	}
	else {
		glEnable(GL_SCISSOR_TEST);
		glScissor(position.x + rect.left, window->getSize().y - position.y - rect.height - rect.top, rect.width, rect.height);
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
	if (!visible) return false;

	switch (e.type)
	{
	case CustomWinEvent::MouseDown:
		if (e.mouse.button == sf::Mouse::Left) mouseDownFunc(this);
		else return false;
		return true;

	case CustomWinEvent::MouseUp:
		if (e.mouse.button == CustomWinEvent::MouseButton::Left && hasPoint(sf::Vector2f(e.mouse.x, e.mouse.y)))
		{
			mouseUpFunc(this);
			return true;
		}
		return false;
	}

	return false;
}

void TextLabel::updateText()
{
	if (rect.left < 0 || align == LEFT)
	{
		text.setPosition(position - offset);
		return;
	}

	sf::FloatRect bound = text.getLocalBounds();
	switch (align)
	{

	case MIDDLE:
		text.setPosition(position.x + (rect.width - bound.width) / 2 - offset.x, position.y - offset.y);
		break;

	case RIGHT:
		text.setPosition(position.x + rect.height - bound.width - offset.x, position.y - offset.y);
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

void TextLabel::setDisplayRect(float x, float y, float width, float height)
{
	rect.left = x;
	rect.top = y;
	rect.width = width;
	rect.height = height;

	updateText();
}

sf::FloatRect TextLabel::getDisplayRect()
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

void TextLabel::SetMouseDownFunction(const std::function<void(TextLabel*)>& func)
{
	this->mouseDownFunc = func;
}

void TextLabel::SetMouseUpFunction(const std::function<void(TextLabel*)>& func)
{
	this->mouseUpFunc = func;
}

void TextLabel::SetOffsetX(float x)
{
	offset.x = x;
	updateText();
}

void TextLabel::SetOffsetY(float y)
{
	offset.y = y;
	updateText();
}

sf::Vector2f TextLabel::GetOffset()
{
	return offset;
}