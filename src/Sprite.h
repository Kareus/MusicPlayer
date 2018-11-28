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

	void updatePosition();

public:
	Sprite(const std::string& texturePath);

	Sprite(const Sprite& data);

	~Sprite();

	void draw(sf::RenderWindow* window) override;

	void SetPosition(float x, float y) override;

	void SetPositionX(float x) override;

	void SetPositionY(float y) override;

	sf::Vector2f GetPosition() override;

	sf::Vector2f GetSize() override;

	bool pollEvent(sf::Event e) override { return false; }
	bool pollEvent(CustomWinEvent e) override { return false; }

	bool hasPoint(const sf::Vector2f& point) override;
};
#endif