#include "TextBox.h"
#include "GlobalFunctions.h"

TextBox::TextBox(float x, float y, float width, float height, bool multi_line)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	multiLine = multi_line;
	onlyNumber = false;
	
	shape = sf::RectangleShape(sf::Vector2f(width, height));
	shape.setPosition(x, y);
	shape.setFillColor(sf::Color::White);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(1);

	font = sf::Font();

	str = L"";
	origin = L"";
	selection = L"";
	text = sf::Text(str, font);
	text.setFillColor(sf::Color::Black);

	focus = false;
	cursorPos = 0;
	maxLen = -1;

	cursor = sf::RectangleShape(sf::Vector2f(2, text.getCharacterSize()));
	cursor.setPosition(x, y + 2);
	cursor.setFillColor(sf::Color::Black);
	blink = true;
	input = false;
	mouseOver = false;
}

void TextBox::draw(sf::RenderWindow* window)
{
	window->draw(shape);

	glEnable(GL_SCISSOR_TEST); //Ŭ����
	glScissor(x, window->getSize().y - height - y, width, height); //openGL�� SFML�� ��ǥ�谡 ���� �����Ǿ� �����Ƿ� ������Ų��.
	window->draw(text);
	glDisable(GL_SCISSOR_TEST); //Ŭ���� ����

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
	std::wstring filtered = L"";

	for (int i = 0; i < str.size(); i++)
	{
		wchar_t ch = str.at(i);
		if (onlyNumber && (ch < 48 || ch > 57)) continue; //���ڰ� �ƴ� �����Ͱ� �ִ� ��� ����
		if (multiLine && ch == L'\n') continue; //���� ���� ������ �ʴ� ��� ����
		filtered.push_back(ch);
	}

	if (maxLen >= 0) this->str = filtered.substr(0, maxLen);
	else this->str = filtered;

	cursorPos = (this->str).size();
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

void TextBox::setOnlyNumber(bool only)
{
	onlyNumber = only;

	if (onlyNumber)
	{
		std::wstring newStr = L"";
		for (int i = 0; i < str.size(); i++)
		{
			wchar_t ch = str.at(i);
			if (ch >= 48 && ch <= 57) newStr.push_back(ch);
		}

		setText(newStr);
	}
}

bool TextBox::pollEvent(CustomWinEvent e)
{
	if (str.size() >= maxLen)
	{
		input = false;
		str = str.substr(0, maxLen);
		updateText();

		INPUT ip;
		ip.type = INPUT_KEYBOARD;
		ip.ki.time = 0;
		ip.ki.dwExtraInfo = 0;
		ip.ki.wVk = VK_RIGHT;
		ip.ki.dwFlags = 0;
		SendInput(1, &ip, sizeof(INPUT)); //IME ���Ḧ ���� �������� ������ ȭ��ǥ Ű�� �������� ��ȣ�� �����Ѵ�
		return false;
	}

	if (focus)
	{
		switch (e.type)
		{
		case CustomWinEvent::MouseDown:
			if (e.mouse.button == CustomWinEvent::MouseButton::Left)
			{
				timer.restart();

				for (int i = 1; i < str.size(); i++) //Ŀ�� ��ġ ã��
				{
					if (text.findCharacterPos(i).x > e.mouse.x)
					{
						cursorPos = i - 1;
						updateText();
						return true;
					}
				}

				cursorPos = str.size();
				updateText();
				return true;
			}
			return false;

		case CustomWinEvent::TextEntered:
			return textEvent(e.text.unicode);

		case CustomWinEvent::KeyPressed:
			switch (e.key.code)
			{
			case VK_LEFT: //���� ȭ��ǥ
				if (cursorPos > 0) cursorPos--;
				break;

			case VK_RIGHT: //������ ȭ��ǥ
				if (cursorPos < str.size()) cursorPos++;
				break;

			case 'D': //ctrl+D. ��ü ����
				if (e.key.control) setText(L"");
				break;

			case 'X': //ctrl+X. ��ü �߶󳻱�
				if (e.key.control)
				{
					const char* output = String::WstrToStr(str).c_str();
					const size_t len = strlen(output) + 1;
					HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
					memcpy(GlobalLock(hMem), output, len);
					GlobalUnlock(hMem);
					OpenClipboard(0);
					EmptyClipboard();
					SetClipboardData(CF_TEXT, hMem);
					CloseClipboard();
					setText(L"");
				}
				break;

			case 'C': //ctrl+C. ��ü ����
				if (e.key.control)
				{
					const char* output = String::WstrToStr(str).c_str();
					const size_t len = strlen(output) + 1;
					HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
					memcpy(GlobalLock(hMem), output, len);
					GlobalUnlock(hMem);
					OpenClipboard(0);
					EmptyClipboard();
					SetClipboardData(CF_TEXT, hMem);
					CloseClipboard();
				}
				break;

			case 'V': //ctrl+V. �ٿ��ֱ�
				if (e.key.control)
				{
					if (!OpenClipboard(NULL)) return false;
					std::string newStr = (char*)GetClipboardData(CF_TEXT);
					setText(str + String::StrToWstr(newStr));
					CloseClipboard();
				}
				break;

			default:
				return false;
			}

			updateText();
			return true;

		case CustomWinEvent::IMEComposing: //ime ���ڸ� �����ϴ� ��
			font.getGlyph(e.ime.code, text.getCharacterSize(), false); //�ش� �۸����� ��Ʈ���� �ε��Ѵ�.
			if (!input)
			{
				origin = str; //���� ���ڿ� ����
				if (cursorPos < origin.size()) str = origin.substr(0, cursorPos) + e.ime.code + origin.substr(cursorPos);
				else str = origin + e.ime.code; //�ش� ime �ڵ带 ���ڿ��� ����
				input = true; //�Է� ������ true�� ����
			}
			else
			{
				if (cursorPos < origin.size()) str = origin.substr(0, cursorPos) + e.ime.code + origin.substr(cursorPos);
				else str = origin + e.ime.code;
			}
			updateText();
			return true;

		case CustomWinEvent::IMEEnd: //ime ���� �� ���ڸ� �ϼ���
			input = false; //�Է� ������ false�� ����
			if (cursorPos < origin.size()) str = origin.substr(0, cursorPos) + e.ime.code + origin.substr(cursorPos);
			else str = origin + e.ime.code; //���� ���� ���ڸ� ���ڿ��� ����
			cursorPos++; //Ŀ�� ������ ����
			origin = str;
			updateText();
			return true;

		case CustomWinEvent::IMEResult:
			if (!input) return false; //ime �Է� ���� �ƴ� ��� false ��ȯ

			//Ư���� ��Ȳ���� ���������� ������ �ϼ����� ���ϰ� �Է��� ����� ��츦 ó���Ѵ�.
			if (GetAsyncKeyState(VK_BACK)) //backspace Ű
				str = origin.substr(0, cursorPos) + origin.substr(cursorPos);

			updateText();
			return true;
		}
	}
	else
	{
		switch (e.type)
		{
		case CustomWinEvent::MouseMoved:
			if (mouseOver && !hasPoint(sf::Vector2f(e.mouse.x, e.mouse.y)))
			{
				mouseOver = false;
				SetCursor(LoadCursor(NULL, IDC_ARROW));
				return true;
			}
			return false;

		case CustomWinEvent::MouseOver:
			SetCursor(LoadCursor(NULL, IDC_IBEAM));
			mouseOver = true;
			return true;
		}
	}

	return false;
}

bool TextBox::textEvent(sf::Uint32 code)
{
	if (code == 8) //backspace Ű
	{
		if (cursorPos <= 0) return false;
		if (cursorPos < str.size()) str = str.substr(0, cursorPos - 1) + str.substr(cursorPos);
		else str.pop_back();

		origin = str;
		cursorPos--;
	}
	else
	{
		if (code == 10 || code == 13) //enter Ű
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
		else if (code < 127) //ascii (���ĺ�, ���� ��) Ű
		{
			if (maxLen < 0 || str.size() < maxLen)
			{
				if (onlyNumber && (code < 48 || code > 57)) return false; //���� ���� �Ǵ�

				if (cursorPos < str.size()) str = str.substr(0, cursorPos) + (wchar_t)code + str.substr(cursorPos); //�ٸ� ���	
				else str += (wchar_t)code;
				cursorPos++;
			}
		}
	}
	updateText();

	return true;
}

float TextBox::getCursorPosX()
{
	if (cursorPos == 0 || str.size() == 0) return 0;

	int pos = cursorPos - 1;
	if (input) pos++;
	
	if (pos >= str.size()) pos = str.size() - 1;

	wchar_t ch = str.at(pos);
	if (ch == L' ') ch = L'a'; //������ ũ�⸦ �ν����� ���ϹǷ� a�� ��ü
	float glyph = font.getGlyph(ch, text.getCharacterSize(), false).bounds.width;

	return text.findCharacterPos(pos).x - text.findCharacterPos(0).x + glyph;
}

void TextBox::updateText()
{
	text.setString(str);

	if (cursorPos < 0) cursorPos = 0;
	if (cursorPos > str.size()) cursorPos = str.size();

	float end = getCursorPosX();
	float over = end - width; //Ŀ���� �ش��ϴ� ���� ��ġ - �ڽ� �ʺ�

	if (over >= 0) text.setPosition(x - over, y); //0���� ũ�� �ؽ�Ʈ ���̰� �ڽ����� �� ���̹Ƿ� ��ġ ����
	else text.setPosition(x, y); //�ƴ� ��� ���� ��ġ�� ����.

	if (input && cursorPos == 0 && str.size() > 0) end += font.getGlyph(str.at(0), text.getCharacterSize(), false).bounds.width; //���� ������ insert �ÿ� ����
	if (over >= 0) cursor.setPosition(x - over + end, y + 2);
	else cursor.setPosition(x + end, y + 2);
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
	cursor.setFillColor(sf::Color(255 - color.r, 255 - color.g, 255 - color.b)); //����� �������� ����
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
	cursor.setSize(sf::Vector2f(2, text.getCharacterSize()));
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