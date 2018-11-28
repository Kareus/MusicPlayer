#include "TextBox.h"
#include "GlobalFunctions.h"

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

	cursorColor = sf::Color::Black;
	cursor = sf::RectangleShape(sf::Vector2f(2, text.getCharacterSize()));
	cursor.setPosition(x, y + 2);
	cursor.setFillColor(cursorColor);
	blink = true;
	input = false;
}

void TextBox::draw(sf::RenderWindow* window)
{
	window->draw(shape);
	glEnable(GL_SCISSOR_TEST); //클리핑
	glScissor(x, window->getSize().y-height, width, height); //openGL과 SFML의 좌표계가 상하 반전되어 있으므로 반전시킨다.
	window->draw(text);
	glDisable(GL_SCISSOR_TEST); //클리핑 종료

	if (focus)
	{
		float time = timer.getElapsedTime().asMilliseconds();
		
		if (time >= BLINK)
		{
			timer.restart();
			blink = !blink;
		}

		if (blink) window->draw(cursor);
	}
}

void TextBox::setText(const std::wstring& str)
{
	this->str = str;
	cursorPos = str.size();
	updateText();
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

bool TextBox::pollEvent(sf::Event e)
{
	if (focus)
	{
		switch (e.type)
		{
		case sf::Event::TextEntered:
			return textEvent(e.text.unicode);

		case sf::Event::KeyPressed:
			if (e.key.code == VK_LEFT)
			{
				if (cursorPos > 0) cursorPos--;
			}
			else if (e.key.code == VK_RIGHT)
			{

				if (cursorPos < str.size()) cursorPos++;
			}
			else return false;

			updateText();
			return true;
		}
	}

	if (e.type == sf::Event::MouseButtonPressed)
		if (e.mouseButton.button == sf::Mouse::Left)
		{
			setFocus(true);
			timer.restart();
			return true;
		}

	return false;
}

bool TextBox::pollEvent(CustomWinEvent e)
{
	if (focus)
	{
		switch (e.type)
		{
		case CustomWinEvent::IMEComposing:
			font.getGlyph(e.ime.code, text.getCharacterSize(), false);
			if (maxLen < 0 || str.size() < maxLen)
			{
				if (!input)
				{
					if (cursorPos < str.size()) str = str.substr(0, cursorPos) + e.ime.code + str.substr(cursorPos);
					else str += e.ime.code;
					input = true;
				}
				else
					if (cursorPos + 1 < str.size()) str = str.substr(0, cursorPos) + e.ime.code + str.substr(cursorPos+1);
					else str = str.substr(0, cursorPos) + e.ime.code;
				updateText();
				return true;
			}
			return false;

		case CustomWinEvent::IMEEnd:
			input = false;
			cursorPos++;
			if (cursorPos < str.size()) str = str.substr(0, cursorPos) + e.ime.code + str.substr(cursorPos);
			else str = str.substr(0, cursorPos-1);
			updateText();
			return true;
		}
	}

	return false;
}

bool TextBox::textEvent(sf::Uint32 code)
{
	if (code == 8)
	{
		if (cursorPos <= 0) return false;
		if (cursorPos < str.size()) str = str.substr(0, cursorPos - 1) + str.substr(cursorPos);
		else str.pop_back();

		cursorPos--;
	}
	else
	{
		if (code == 10 || code == 13)
		{
			if (!multiLine) return false;
			else
			{
				if (maxLen < 0 || str.size() < maxLen)
				{
					str = str.substr(0, cursorPos) + L'\n' + str.substr(cursorPos);
					cursorPos++;
				}
				height += text.getCharacterSize();
				shape.setSize(sf::Vector2f(width, height));
			}
		}
		else if (maxLen < 0 || str.size() < maxLen)
		{
			if (cursorPos < str.size()) str = str.substr(0, cursorPos) + (wchar_t)code + str.substr(cursorPos); //다른 경우	
			else str += (wchar_t)code;
			font.getGlyph(code, text.getCharacterSize(), false);
			cursorPos++;
		}
	}
	updateText();

	return true;
}

void TextBox::updateText()
{
	text.setString(str);
	if (cursorPos < 0) cursorPos = 0;
	if (cursorPos > str.size()) cursorPos = str.size();

	float end = text.findCharacterPos(cursorPos).x;
	if (input) end = text.findCharacterPos(cursorPos + 1).x;

	float over = end - text.findCharacterPos(0).x - width; //커서에 해당하는 글자 위치 - 박스 너비
	if (over >= 0) text.setPosition(x - over, y); //0보다 크면 텍스트 길이가 박스보다 긴 것이므로 위치 조정
	else text.setPosition(x, y); //아닌 경우 원래 위치로 리셋.

	cursor.setPosition(end, y + 2);
}

bool TextBox::hasPoint(const sf::Vector2f& point)
{
	return x <= point.x && point.x < x + width && y <= point.y && point.y < y + height;
}

void TextBox::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	updateText();
}

void TextBox::SetPositionX(float x)
{
	this->x = x;
	updateText();
}

void TextBox::SetPositionY(float y)
{
	this->y = y;
	updateText();
}

sf::Vector2f TextBox::GetPosition()
{
	return sf::Vector2f(x, y);
}

sf::Vector2f TextBox::GetSize()
{
	return sf::Vector2f(width, height);
}

void TextBox::setBackgroundColor(sf::Color color)
{
	shape.setFillColor(color);
}

void TextBox::setBorderColor(sf::Color color)
{
	shape.setOutlineColor(color);
}

void TextBox::setBorderSize(float size)
{
	shape.setOutlineThickness(size);
}

void TextBox::setTextColor(sf::Color color)
{
	text.setFillColor(color);
}

sf::Color TextBox::getBackgroundColor()
{
	return shape.getFillColor();
}

sf::Color TextBox::getBorderColor()
{
	return shape.getOutlineColor();
}

float TextBox::getBorderSize()
{
	return shape.getOutlineThickness();
}

sf::Color TextBox::getTextColor()
{
	return text.getFillColor();
}

void TextBox::setMaxLength(unsigned int len)
{
	maxLen = len;
	if (maxLen >= 0) setText(str.substr(0, len));
}

unsigned int TextBox::getMaxLength()
{
	return maxLen;
}

void TextBox::setCharacterSize(unsigned int size)
{
	unsigned int lines = height / text.getCharacterSize();
	text.setCharacterSize(size);
	height = lines * size; //height 재설정
	shape.setSize(sf::Vector2f(width, height));
}

unsigned int TextBox::getCharacterSize()
{
	return text.getCharacterSize();
}

bool TextBox::loadFontFrom(const std::string& filepath)
{
	return font.loadFromFile(filepath);
}

sf::Font& TextBox::getFont()
{
	return font;
}