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

	/**
	*	@brief	�׷����� ��ǥ�� �����ϴ� �Լ�
	*	@param	x	x��ǥ
	*/
	virtual void SetPositionX(float x) = 0;

	/**
	*	@brief	�׷����� ��ǥ�� �����ϴ� �Լ�
	*	@param	y	y��ǥ
	*/
	virtual void SetPositionY(float y) = 0;

	/**
	*	@brief	�׷����� ��ǥ�� ��ȯ�ϴ� �Լ�
	*	@return	�׷����� ��ǥ
	*/
	virtual sf::Vector2f GetPosition() = 0;

	/**
	*	@brief	�׷����� ũ�⸦ ��ȯ�ϴ� �Լ�
	*	@return	�׷����� ũ��
	*/
	virtual sf::Vector2f GetSize() = 0;

	/**
	*	@brief ǥ���ϰԲ� �����ϴ� �Լ�
	*/
	void Show() { visible = true; }

	/**
	*	@brief	�׷����� ǥ������ �ʰԲ� �����ϴ� �Լ�
	*/
	void Hide() { visible = false; }

	/**
	*	@brief	�׷����� ǥ�õǴ��� ���θ� ��ȯ�ϴ� �Լ�
	*	@return	�׷����� ǥ�õǸ� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool isVisible() { return visible; }

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	g	���� ������
	*	@return	���� �����Ͱ� �� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator<(const Graphic& g) const { return id < g.id; }

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ū�� ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	g	���� ������
	*	@return	���� �����Ͱ� �� ũ�� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator>(const Graphic& g) const { return id > g.id; }

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����Ϳ� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	g	���� ������
	*	@return	���� �����Ϳ� ���ϴ� �����Ͱ� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator==(const Graphic& g) const { return id == g.id; }

	/**
	*	@brief	�׷����� ���� �����͸� �����ϴ� �Լ�
	*	@param	data	������ ������
	*/
	void SetData(const std::string& data) { this->data = data; }

	/**
	*	@brief	�׷����� �����͸� ��ȯ�ϴ� �Լ�
	*	@return	����� ������
	*/
	std::string GetData() { return data; }
};
#endif