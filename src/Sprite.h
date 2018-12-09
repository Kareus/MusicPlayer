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
* @author �輺��
*/
class Sprite : public Graphic
{
private:
	sf::Sprite* sprite; ///<������ �׷����� ����ϴ� ��������Ʈ ������ ����.
	std::shared_ptr<sf::Texture> texture; ///<�ؽ�ó�� �����ϴ� ������ ����. ���� �ؽ�ó�� ���� ��ü�� ������ �� �����Ƿ� shared pointer�� ������.
	sf::Vector2f position; ///<��������Ʈ�� ��ġ ����.
	std::function<void(Sprite*)> mouseDownFunc; ///<���콺�� ������ �� ������ �Լ�
	std::function<void(Sprite*)> mouseUpFunc; ///<���콺�� ���� �� ������ �Լ�
	sf::Color overColor;
	sf::Color normalColor;
	sf::IntRect textureRect;
	bool mouseOver;
	bool button;

	void updatePosition();

	Sprite() {}; ///<�⺻ �����ڴ� �ܺο��� ����� �� ������ ����

public:
	Sprite(const std::string& texturePath);

	Sprite(const Sprite& data);

	~Sprite();

	void draw(sf::RenderWindow* window) override;

	void SetPosition(float x, float y) override;

	void SetPositionX(float x) override;

	void SetPositionY(float y) override;

	void SetTextureRect(float width, float height);

	void SetTexturePos(float x, float y);

	sf::Vector2f GetPosition() override;

	sf::Vector2f GetTexturePos();

	sf::Vector2f GetSize() override;

	void SetButton(bool b);

	bool isButton() const;

	void SetMouseDownFunction(const std::function<void(Sprite*)>& func);

	void SetMouseUpFunction(const std::function<void(Sprite*)>& func);

	bool pollEvent(CustomWinEvent e) override;

	bool hasPoint(const sf::Vector2f& point) override;

	void ResetMouseOver();

	void TriggerMouseOver();

	Sprite* clone();
};
#endif