#pragma once
#ifndef __TEXT_BOX__
#define __TEXT_BOX__

#include "Graphic.h"
#include <string>
#include <Windows.h>

/**
*	����ڷκ��� �ؽ�Ʈ�� �Է¹��� �� �ִ� �ؽ�Ʈ ���� Ŭ����.
*	@date	2018.11.28
*	@author	�輺��
*/
class TextBox : public Graphic
{
private:
	sf::RectangleShape shape; ///<��� �簢��
	sf::RectangleShape cursor; ///<�ؽ�Ʈ Ŀ��
	sf::Clock timer; ///<Ŀ�� �������� ���� Ÿ�̸�
	sf::Text text; ///<�ؽ�Ʈ
	sf::Font font; ///<��Ʈ
	std::wstring str; ///<�ؽ�Ʈ�� ���ڿ�
	std::wstring origin; ///<IME �Է¿� �� ���� ���ڿ�
	std::wstring selection; ///<���� ���ڿ�
	static const unsigned int BLINK = 500; ///<�����ӿ� �ð� ���
	bool blink; ///<������ ����
	int cursorPos; ///<���� Ŀ�� ��ġ
	unsigned int maxLen; ///<�ִ� ���� (�������� -1)
	bool onlyNumber; ///<���ڸ� �Է¹��� �� ����
	float x; ///<x ��ǥ
	float y; ///<y ��ǥ
	float width; ///<�ʺ�
	float height; ///<����
	bool multiLine; ///<���� �� ���� ����
	bool input; ///<IME �Է� ����
	bool mouseOver; ///<���콺 ���� ����

	/**
	*	@brief	�ؽ�Ʈ ���� �̺�Ʈ�� �����ϴ� �Լ�
	*	@param	code	�Է¹��� Ű�ڵ� ��
	*/
	bool textEvent(sf::Uint32 code);

	/*
	*	�ؽ�Ʈ�� ������Ʈ�ϴ� �Լ�
	*/
	void updateText();

public:

	/**
	*	@brief	�ؽ�Ʈ �ڽ��� �����ϴ� ������
	*	@param	x	x��ǥ
	*	@param	y	y��ǥ
	*	@param	width	�ʺ�
	*	@param	height	��
	*	@param	multi_line	���� �� ��� ����
	*/
	TextBox(float x, float y, float width, float height, bool multi_line = false);

	//�⺻ �Ҹ���
	virtual ~TextBox() {};

	/**
	*	@brief	�����쿡 �׷����� �������ϴ� �Լ�
	*	@param	window	�������� ������
	*/
	void draw(sf::RenderWindow* window) override;

	/**
	*	@brief	�ؽ�Ʈ�� �����ϴ� �Լ�
	*	@param	str	������ �ؽ�Ʈ
	*/
	void setText(const std::wstring& str);

	/**
	*	@brief	��Ʈ�� �����ϴ� �Լ�
	*	@param	font	������ ��Ʈ
	*/
	void setFont(sf::Font& font);

	/**
	*	@brief	�ؽ�Ʈ�� ��ȯ�ϴ� �Լ�
	*	@return	�Էµ� �ؽ�Ʈ
	*/
	std::wstring getText();

	/**
	*	@brief	�̺�Ʈ�� �޾� �� �׷��ȿ��� �����ϴ� �Լ�
	*	@param	e	�̺�Ʈ
	*	@return	�̺�Ʈ�� ���������� ó���ߴ��� ���ΰ� ��ȯ�ȴ�
	*/
	bool pollEvent(CustomWinEvent e) override;

	/**
	*	@brief	�Է¹��� ���� ���� �׷����� ������ ���ԵǴ��� ���θ� ��ȯ�Ѵ�.
	*	@param	point	��ǥ
	*	@return	���ԵǸ� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool hasPoint(const sf::Vector2f& point) override;

	/**
	*	@brief	��Ʈ�� ��ȯ�ϴ� �Լ�
	*	@return	������ ��Ʈ
	*/
	sf::Font& getFont();

	/**
	*	@brief	�Է¹��� ��ηκ��� ��Ʈ�� �ε��ϴ� �Լ�
	*	@param	filepath	��Ʈ ���
	*/
	bool loadFontFrom(const std::string& filepath);

	/**
	*	@brief	��� ������ �����ϴ� �Լ�
	*	@param	color	������ ��� ����
	*/
	void setBackgroundColor(sf::Color color);

	/**
	*	@brief	�׵θ� ������ �����ϴ� �Լ�
	*	@param	color	������ �׵θ� ����
	*/
	void setBorderColor(sf::Color color);

	/**
	*	@brief	�׵θ� ũ�⸦ �����ϴ� �Լ�
	*	@param	size	������ �׵θ� ũ��
	*/
	void setBorderSize(float size);

	/**
	*	@brief	�ؽ�Ʈ ������ �����ϴ� �Լ�
	*	@param	color	������ �ؽ�Ʈ ����
	*/
	void setTextColor(sf::Color color);

	/**
	*	@brief	���� ũ�⸦ �����ϴ� �Լ�
	*	@param	size	������ ���� ũ��
	*/
	void setCharacterSize(unsigned int size);

	/**
	*	@brief	��� ������ ��ȯ�Ѵ�.
	*	@return	��� ����
	*/
	sf::Color getBackgroundColor();

	/**
	*	@brief	�׵θ� ������ ��ȯ�Ѵ�.
	*	@return	�׵θ� ����
	*/
	sf::Color getBorderColor();

	/**
	*	@brief	�׵θ� ũ�⸦ ��ȯ�Ѵ�.
	*	@return	�׵θ� ũ��
	*/
	float getBorderSize();

	/**
	*	@brief	���� ������ ��ȯ�Ѵ�.
	*	@return	���� ����
	*/
	sf::Color getTextColor();

	/**
	*	@brief	���� ũ�⸦ ��ȯ�Ѵ�.
	*	@return	���� ũ��
	*/
	unsigned int getCharacterSize();

	/**
	*	@brief	�ؽ�Ʈ Ŀ�� ��ġ�� ��ȯ�Ѵ�.
	*	@return	�ؽ�Ʈ Ŀ�� ��ġ
	*/
	float getCursorPosX();

	/**
	*	@brief	�ִ� ���� ���� �����Ѵ�.
	*	@param	len	������ �ִ� ���� ��
	*/
	void setMaxLength(unsigned int len);

	/**
	*	@brief	�ִ� ���� ���� ��ȯ�Ѵ�.
	*	@return	�ִ� ���� ��
	*/
	unsigned int getMaxLength();

	/**
	*	@brief	���ڸ� �Է¹����� �����Ѵ�.
	*	@param	only	���ڸ� �Է¹����� ����
	*/
	void setOnlyNumber(bool only);

	/**
	*	@brief	���ڸ� �Է¹޴��� ���θ� ��ȯ�Ѵ�.
	*	@return	���ڸ� �Է¹����� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool IsOnlyNumber();

	/**
	*	@brief	�׷����� ��ġ�� �����ϴ� �Լ�
	*	@param	x	x��ǥ
	*	@param	y	y��ǥ
	*/
	void SetPosition(float x, float y) override;

	/**
	*	@brief	�׷����� ��ġ�� �����ϴ� �Լ�
	*	@param	x	x��ǥ
	*/
	void SetPositionX(float x) override;

	/**
	*	@brief	�׷����� ��ġ�� �����ϴ� �Լ�
	*	@param	y	y��ǥ
	*/
	void SetPositionY(float y) override;

	/**
	*	@brief	�׷����� ��ġ�� ��ȯ�ϴ� �Լ�
	*	@return	�׷����� ��ġ.
	*/
	sf::Vector2f GetPosition() override;

	/**
	*	@brief	�׷����� ũ�⸦ ��ȯ�ϴ� �Լ�
	*	@return	�׷����� ũ��.
	*/
	sf::Vector2f GetSize() override;

};
#endif