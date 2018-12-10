#pragma once
#ifndef __GRAPHIC__
#define __GRAPHIC__

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "CustomWinEvent.h"
#include <string>

/**
*	@brief	�������� �׷��ȿ� ���õ� base class (�߻� Ŭ�����̴�)
*	@author	�輺��
*/
class Graphic
{
protected:
	int id; ///<�׷����� ������ primary key
	bool focus; ///<�׷����� ���� ��Ŀ���� �ް� �ִ��� ����
	std::string data; ///< application���� �����͸� ã�� ���� ����
	bool visible; ///<�׷����� ���������� ����

public:
	/**
	*	�⺻ ������
	*/
	Graphic() : id(0), focus(false), visible(true) {};

	/**
	*	�⺻ �Ҹ���
	*/
	virtual ~Graphic() {}; //virtual�� �����Ͽ� dynamic binding�� ��ü�� �ùٸ� �Ҹ��ڸ� ȣ���� �� �ִ�.

	/**
	*	@brief	�����쿡 �������ϱ� ���� ���� �����Լ�
	*	@param	window	�������� ������
	*/
	virtual void draw(sf::RenderWindow* window) = 0;

	/**
	*	@brief	�̺�Ʈ�� �޾� �� �׷��ȿ��� ������ ���� �����Լ�
	*	@param	e	�̺�Ʈ
	*	@return	�̺�Ʈ�� ���������� ó���ߴ��� ���ΰ� ��ȯ�ȴ�
	*/
	virtual bool pollEvent(CustomWinEvent e) = 0;

	/**
	*	@brief	�Է¹��� ���� ���� �׷����� ������ ���ԵǴ��� ���θ� ��ȯ�ϴ� ���� �����Լ�
	*	@param	point	��ǥ
	*	@return	���ԵǸ� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	virtual bool hasPoint(const sf::Vector2f& point) = 0;

	/**
	*	@brief	primary key �����ϴ� �Լ�
	*	@param	id	primary key
	*/
	void setID(int id) { this->id = id; }

	/**
	*	@brief	primary key�� ��ȯ�ϴ� �Լ�
	*	@return	primary key
	*/
	int getID() const { return id; }

	/**
	*	@brief	��Ŀ���� �����ϴ� �Լ�
	*	@param	f	��Ŀ�� ����
	*/
	void setFocus(bool f) { focus = f; }

	/**
	*	@brief	�׷����� ��ǥ�� �����ϴ� �Լ�
	*	@param	x	x��ǥ
	*	@param	y	y��ǥ
	*/
	virtual void SetPosition(float x, float y) = 0;

	virtual void SetPositionX(float x) = 0;
	virtual void SetPositionY(float y) = 0;
	virtual sf::Vector2f GetPosition() = 0;
	virtual sf::Vector2f GetSize() = 0;
	void Show() { visible = true; }
	void Hide() { visible = false; }
	bool isVisible() { return visible; }

	bool operator<(const Graphic& g) const { return id < g.id; }
	bool operator>(const Graphic& g) const { return id > g.id; }
	bool operator==(const Graphic& g) const { return id == g.id; }

	void SetData(const std::string& data) { this->data = data; }
	std::string GetData() { return data; }
};
#endif