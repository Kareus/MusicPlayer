#include "TextBox.h"
#include "GlobalFunctions.h"

TextBox::TextBox(float x, float y, float width, float height, HWND parent, HINSTANCE instance, bool multi_line)
{
	multiLine = multi_line;

	position.x = x;
	position.y = y;
	size.x = width;
	size.y = height;

	HWND box = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, x, y, width, height, parent, NULL, instance, NULL);;
	textBox.create(box);
}

void TextBox::draw(sf::RenderWindow* window)
{
	SetWindowPos(window->getSystemHandle(), textBox.getSystemHandle(), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

void TextBox::setText(const std::wstring& str)
{
	updateText();
}

std::wstring TextBox::getText()
{
	return L"";
}

void TextBox::setFont(sf::Font& font)
{
	
}

bool TextBox::pollEvent(sf::Event e)
{
	return false;
}

bool TextBox::pollEvent(CustomWinEvent e)
{
	return false;
}

bool TextBox::textEvent(sf::Uint32 code)
{
	return false;
}

void TextBox::updateText()
{

}

bool TextBox::hasPoint(const sf::Vector2f& point)
{
	return position.x <= point.x && point.x < position.x + size.x && position.y <= point.y && point.y < position.y + size.y;
}

void TextBox::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	updateText();
}

void TextBox::SetPositionX(float x)
{
	position.x = x;
	updateText();
}

void TextBox::SetPositionY(float y)
{
	position.y = y;
	updateText();
}

sf::Vector2f TextBox::GetPosition()
{
	return position;
}

sf::Vector2f TextBox::GetSize()
{
	return size;
}

void TextBox::setBackgroundColor(sf::Color color)
{
	
}

void TextBox::setBorderColor(sf::Color color)
{
	
}

void TextBox::setBorderSize(float size)
{
	
}

void TextBox::setTextColor(sf::Color color)
{
	
}

sf::Color TextBox::getBackgroundColor()
{
	return sf::Color::Transparent;
}

sf::Color TextBox::getBorderColor()
{
	return sf::Color::Transparent;
}

float TextBox::getBorderSize()
{
	return 0;
}

sf::Color TextBox::getTextColor()
{
	return sf::Color::Transparent;
}

void TextBox::setMaxLength(unsigned int len)
{
	maxLen = len;
	//if (maxLen >= 0) setText(str.substr(0, len));
}

unsigned int TextBox::getMaxLength()
{
	return maxLen;
}

void TextBox::setCharacterSize(unsigned int size)
{
	
}

unsigned int TextBox::getCharacterSize()
{
	return 0;
}

bool TextBox::loadFontFrom(const std::string& filepath)
{
	return false;
}