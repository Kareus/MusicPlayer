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
	glEnable(GL_SCISSOR_TEST); //Ŭ����
	glScissor(x, window->getSize().y-height, width, height); //openGL�� SFML�� ��ǥ�谡 ���� �����Ǿ� �����Ƿ� ������Ų��.
	window->draw(text);
	glDisable(GL_SCISSOR_TEST); //Ŭ���� ����
}

void TextBox::setText(const std::wstring& str)
{
	this->str = str;
	cursorPos = str.size() - 1;
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

		str = str.substr(0, cursorPos - 1); //����
		cursorPos--;
	}
	else
	{
		if (code == 10 || code == 13)
		{
			if (!multiLine) return true;
			else
			{
				str += L'\n';
				height += text.getCharacterSize();
				shape.setSize(sf::Vector2f(width, height));
			}
		}
		else str += (wchar_t)code; //�ٸ� ���	
		cursorPos++;
	}

	updateText();

	return false;
}

void TextBox::updateText()
{
	text.setString(str);
	float over = text.findCharacterPos(cursorPos).x - text.findCharacterPos(0).x - width; //Ŀ���� �ش��ϴ� ���� ��ġ - �ڽ� �ʺ�
	if (over >= 0) text.setPosition(x - over, y); //0���� ũ�� �ؽ�Ʈ ���̰� �ڽ����� �� ���̹Ƿ� ��ġ ����
	else text.setPosition(x, y); //�ƴ� ��� ���� ��ġ�� ����.
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