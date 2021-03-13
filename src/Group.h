#pragma once
#ifndef __GROUP__
#define __GROUP__

#include "Graphic.h"
#include "DoublyLinkedList.h"
#include "DoublyIterator.h"

/**
*	@brief	�׷����� ��ӹ޾� ���� �׷��� �׸���� �׷캰�� �������ϴ� Ŭ����.
*	@author	Kareus
*/
class Group : public Graphic
{
private:
	sf::Vector2f position; ///<�׷����� ��ġ
	DoublyLinkedList<Graphic*> drawings; ///<�׷쿡�� �������� ����Ʈ
	Graphic* focus; ///<��Ŀ���� ������ �׷���
	sf::FloatRect viewRect; ///<�������� ����
	function<int(Graphic* const&, Graphic* const&)> compareGraphics = [](Graphic* const& g1, Graphic* const& g2) {
		return (*g1 > *g2) - (*g1 < *g2); //�����ʹ� �ּҰ��� ���ϹǷ�, ����Ű�� ��ü�� �� ���踦 ��ȯ�Ѵ�.
	};

public:

	/*
	*	�⺻ ������
	*/
	Group();

	/*
	*	�⺻ �Ҹ���
	*/
	~Group();

	/**
	*	@brief	�׷����� �׷쿡 �߰��ϴ� �Լ�
	*	@param	graphic	�߰��� �׷���
	*	@param	inherit	�׷��� data�� �߰��� �׷����� ��ӹ����� ����
	*	@return	�߰��� �����ϸ� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int AddGraphic(Graphic* graphic, bool inherit = true);

	/**
	*	@brief	�׷����� �׷쿡�� �����ϴ� �Լ�
	*	@param	graphic	������ �׷���
	*	@return	������ �����ϸ� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int DeleteGraphic(Graphic* graphic);

	/**
	*	@brief	�׷��� �׷��� ����Ʈ�� ���� �Լ�
	*	@param	clear	�� �׷����� �޸𸮸� �������� ����
	*/
	void MakeEmpty(bool clear = true);

	/**
	*	@brief	�׷��� �׷��� ������ ��ȯ�ϴ� �Լ�
	*	@return	�׷��� ����.
	*/
	int GetLength() const;

	/**
	*	@brief	�����쿡 �׷����� �������ϴ� �Լ�
	*	@param	window	�������� ������
	*/
	void draw(sf::RenderWindow* window) override;

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
	*	@brief	�׷����� ������ ����Ʈ�� �ݺ��ڸ� �����Ͽ� ��ȯ�ϴ� �Լ�
	*	@return	������ ����Ʈ�� �ݺ���
	*/
	DoublyIterator<Graphic*> GetIterator();

	/**
	*	@brief	�׷��� ǥ�� ������ �����ϴ� �Լ�
	*	@param	x	x��ǥ
	*	@param	y	y��ǥ
	*	@param	width	�ʺ�
	*	@param	height	��
	*/
	void SetViewRect(float x, float y, float width, float height);

	/**
	*	@brief	�׷����� ǥ�� ������ ��ȯ�ϴ� �Լ�
	*	@return	�׷����� ǥ�� ����
	*/
	sf::FloatRect GetViewRect();
};
#endif