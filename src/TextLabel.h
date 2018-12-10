#pragma once
#ifndef __TEXT_LABEL__
#define __TEXT_LABEL__

#include "Graphic.h"
#include <string>
#include <functional>
#include <Windows.h>

enum TextAlign
{
	LEFT, MIDDLE, RIGHT
};

class TextLabel : public Graphic
{
private:
	sf::Text text; ///<�ؽ�Ʈ �׷��� ��ü
	sf::Font font; ///<��Ʈ
	std::wstring str; ///<�ؽ�Ʈ�� ���ڿ�
	sf::Vector2f position; ///<��ġ
	sf::FloatRect rect; ///<ǥ�� ����
	sf::Vector2f offset; ///<�ִϸ��̼ǿ� ���̰� ��ǥ
	TextAlign align; ///<����
	std::function<void(TextLabel*)> mouseDownFunc; ///<���콺�� ������ �� ������ �Լ�
	std::function<void(TextLabel*)> mouseUpFunc; ///<���콺�� ���� �� ������ �Լ�

	/*
	*	�ؽ�Ʈ�� ������Ʈ�ϴ� �Լ�
	*/
	void updateText();

public:
	/**
	*	@brief	�ؽ�Ʈ ���� �����ϴ� ������.
	*	@param	str	������ �ؽ�Ʈ
	*/
	TextLabel(const std::wstring& str);

	/*
	*	�⺻ �Ҹ���
	*/
	virtual ~TextLabel() {};

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
	*	@brief	������ �����ϴ� �Լ�
	*	@param	align	������ ���� ����
	*/
	void setAlign(TextAlign align);

	/**
	*	@brief	���� ���¸� ��ȯ�ϴ� �Լ�
	*	@return	���� ����
	*/
	TextAlign getAlign();

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
	*	@brief	ǥ�� ������ �����Ѵ�.
	*	@param	x	x��ǥ
	*	@param	y	y��ǥ
	*	@param	width	�ʺ�
	*	@param	height	��
	*/
	void setDisplayRect(float x, float y, float width, float height);

	/**
	*	@brief	ǥ�� ������ ��ȯ�Ѵ�.
	*	@return	ǥ�� ����
	*/
	sf::FloatRect getDisplayRect();

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
	*	@brief	�׷����� ���̰� ��ǥ�� �����ϴ� �Լ�
	*	@param	x	x��ǥ
	*/
	void SetOffsetX(float x);

	/**
	*	@brief	�׷����� ���̰� ��ǥ�� �����ϴ� �Լ�
	*	@param	y	y��ǥ
	*/
	void SetOffsetY(float y);

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

	/**
	*	@brief	�׷����� ���̰� ��ǥ�� ��ȯ�ϴ� �Լ�
	*	@return	�׷����� ���̰� ��ǥ.
	*/
	sf::Vector2f GetOffset();

	/**
	*	@brief	�׷��� ������ ���콺�� ������ �� ������ �Լ��� �����Ѵ�.
	*	@param	func	������ �Լ�
	*/
	void SetMouseDownFunction(const std::function<void(TextLabel*)>& func);

	/**
	*	@brief	�׷��� ������ ���콺�� ���� �� ������ �Լ��� �����Ѵ�.
	*	@param	func	������ �Լ�
	*/
	void SetMouseUpFunction(const std::function<void(TextLabel*)>& func);

};
#endif