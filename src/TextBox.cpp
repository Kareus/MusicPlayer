#include "TextBox.h"

TextBox::TextBox(float x, float y, float width, float height, bool multi_line)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	multiLine = multi_line;
	
	shape = sf::RectangleShape(sf::Vector2f(width, height));
	shape.setPosition(x, y);
	shape.setFillColor(sf::Color::White);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(1);

	font = sf::Font();

	str = L"";
	text = sf::Text(str, font);
	text.setFillColor(sf::Color::Black);

	focus = false;
	cursorPos = 0;
	maxLen = -1;
}

void TextBox::draw(sf::RenderWindow* window)
{
	window->draw(shape);
	glEnable(GL_SCISSOR_TEST);
	sf::Vector2f pos = sf::Vector2f(x, y);
	//glScissor(pos.x, target.getSize().y - (pos.y + getSize().y), getSize().x, getSize().y);
	//target.draw(m_text, states);

	glDisable(GL_SCISSOR_TEST);

	window->draw(text);
}

void TextBox::setText(const std::wstring& str)
{
	this->str = str;
	text.setString(str);
	cursorPos = str.size() - 1;
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

bool TextBox::pollEvent(sf::Event e)
{
	if (focus)
	{
		if (e.type == sf::Event::TextEntered)
			return textEvent(e.text.unicode);
	}

	if (e.type == sf::Event::MouseButtonPressed)
		if (e.mouseButton.button == sf::Mouse::Left)
		{
			setFocus(true);
			return true;
		}

	return false;
}

bool TextBox::textEvent(sf::Uint32 code)
{
	if (code == 8)
	{
		if (cursorPos <= 0) return false;

		str = str.substr(0, cursorPos - 1); //삭제
		cursorPos--;
	}
	else
	{
		if (code == 10 || code == 13)
		{
			if (!multiLine) return true;
			else str += L'\n';
		}
		else str += (wchar_t)code; //다른 경우	
		cursorPos++;
	}

	text.setString(str);
	return false;
}