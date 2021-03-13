#pragma once
#ifndef __SPRITE__
#define __SPRITE__

#include "Graphic.h"
#include <functional>
#include <memory>
#include <thread>
#include <string>

/**
* �׷��� ������ ���� �ؽ�ó�� shared-pointer�� ������ Ŭ����.
* @date 2018.11.26
* @author Kareus
*/
class Sprite : public Graphic
{
private:
	sf::Sprite* sprite; ///<������ �׷����� ����ϴ� ��������Ʈ ������ ����.
	std::shared_ptr<sf::Texture> texture; ///<�ؽ�ó�� �����ϴ� ������ ����. ���� �ؽ�ó�� ���� ��ü�� ������ �� �����Ƿ� shared pointer�� ������.
	sf::Vector2f position; ///<��������Ʈ�� ��ġ ����.
	std::function<void(Sprite*)> mouseDownFunc; ///<���콺�� ������ �� ������ �Լ�
	std::function<void(Sprite*)> mouseUpFunc; ///<���콺�� ���� �� ������ �Լ�
	sf::Color overColor; ///<���콺�� ���� ���� �� ǥ���� ����
	sf::Color normalColor; ///<�Ϲ����� ǥ�� ����
	sf::IntRect textureRect; ///<�ؽ�ó���� ������ ���� ũ��
	bool mouseOver; ///<���콺 ���� ����
	bool button; ///<��ư ����

	/**
	*	@brief	��ġ�� �����ϴ� �Լ�
	*/
	void updatePosition();

	/**
	*	�⺻ ������
	*/
	Sprite() {}; ///<�⺻ �����ڴ� �ܺο��� ����� �� ������ ���� (sprite �� texture ������ ����)

public:
	/**
	*	@brief	�ش� ����� �̹��� ������ �о�� �׷����� �����ϴ� ������
	*	@param	texturePath	�ؽ�ó ���
	*/
	Sprite(const std::string& texturePath);

	/**
	*	���� ������
	*/
	Sprite(const Sprite& data);

	/**
	*	�⺻ �Ҹ���
	*/
	~Sprite();

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
	*	@brief	�׷����� �ؽ�ó ũ�⸦ �����ϴ� �Լ�
	*	@param	width	�ʺ�
	*	@param	height	��
	*/
	void SetTextureRect(float width, float height);

	/**
	*	@brief	�׷����� �ؽ�ó ��ġ�� �����ϴ� �Լ�
	*	@param	x	x��ǥ
	*	@param	y	y��ǥ
	*/
	void SetTexturePos(float x, float y);

	/**
	*	@brief	�׷����� ��ġ�� ��ȯ�ϴ� �Լ�
	*	@return	�׷����� ��ġ.
	*/
	sf::Vector2f GetPosition() override;

	/**
	*	@brief	�׷����� �ؽ�ó ��ġ�� ��ȯ�ϴ� �Լ�
	*	@return	�׷����� �ؽ�ó ��ġ.
	*/
	sf::Vector2f GetTexturePos();

	/**
	*	@brief	�׷����� ũ�⸦ ��ȯ�ϴ� �Լ�
	*	@return	�׷����� ũ��.
	*/
	sf::Vector2f GetSize() override;

	/**
	*	@brief	�׷����� ��ư ���θ� �����ϴ� �Լ�
	*	@param	b	��ư ����
	*/
	void SetButton(bool b);

	/**
	*	@brief	�׷����� ��ư���� ���θ� ��ȯ�Ѵ�.
	*	@return	��ư�̸� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool isButton() const;

	/**
	*	@brief	�׷��� ������ ���콺�� ������ �� ������ �Լ��� �����Ѵ�.
	*	@param	func	������ �Լ�
	*/
	void SetMouseDownFunction(const std::function<void(Sprite*)>& func);

	/**
	*	@brief	�׷��� ������ ���콺�� ���� �� ������ �Լ��� �����Ѵ�.
	*	@param	func	������ �Լ�
	*/
	void SetMouseUpFunction(const std::function<void(Sprite*)>& func);

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
	*	@brief	���콺 ���� ���¸� �����Ѵ�.
	*/
	void ResetMouseOver();

	/**
	*	@brief	���콺 ���� �̺�Ʈ�� �����Ѵ�.
	*/
	void TriggerMouseOver();

	/**
	*	@brief	�׷����� ���纻�� ��ȯ�Ѵ�.
	*	@return	�׷����� ���纻
	*/
	Sprite* clone();
};
#endif