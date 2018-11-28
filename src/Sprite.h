#pragma once
#ifndef __SPRITE__
#define __SPRITE__

#include "Graphic.h"
#include <functional>
#include <memory>
#include <thread>
#include <string>

/**
* 그래픽 구현을 위해 텍스처를 shared-pointer로 가지는 클래스.
* @date 2018.11.26
* @author 김성주
*/
class Sprite : public Graphic
{
private:
	sf::Sprite* sprite; ///<실제로 그래픽을 출력하는 스프라이트 포인터 변수.
	std::shared_ptr<sf::Texture> texture; ///<텍스처를 보관하는 포인터 변수. 같은 텍스처를 여러 객체가 공유할 수 있으므로 shared pointer로 선언함.
	sf::Vector2f position; ///<스프라이트의 위치 변수.

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