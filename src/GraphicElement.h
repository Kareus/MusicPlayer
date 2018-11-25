#pragma once
#ifndef __GRAPHIC_ELEMENT__
#define __GRAPHIC_ELEMENT__

#include <SFML\Graphics.hpp>
#include <functional>
#include <memory>
#include <thread>
#include <string>

/**
* 그래픽 구현을 위해 텍스처를 shared-pointer로 가지는 클래스.
* @date 2018.11.26
* @author 김성주
*/
class GraphicElement
{
private:
	sf::Sprite* sprite; ///<실제로 그래픽을 출력하는 스프라이트 포인터 변수.
	std::shared_ptr<sf::Texture> texture; ///<텍스처를 보관하는 포인터 변수. 같은 텍스처를 여러 객체가 공유할 수 있으므로 shared pointer로 선언함.
	sf::Vector2f position; ///<스프라이트의 위치 변수.

	void updatePosition();

public:
	GraphicElement(const std::string& texturePath);

	GraphicElement(const GraphicElement& data);

	~GraphicElement();

	sf::Sprite* GetSprite() const;

	void SetPosition(float x, float y);

	void SetPositionX(float x);

	void SetPositionY(float y);

	sf::Vector2f GetPosition();
};
#endif