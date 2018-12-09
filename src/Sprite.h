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
	std::function<void(Sprite*)> mouseDownFunc; ///<마우스를 눌렀을 때 실행할 함수
	std::function<void(Sprite*)> mouseUpFunc; ///<마우스를 뗐을 때 실행할 함수
	sf::Color overColor;
	sf::Color normalColor;
	sf::IntRect textureRect;
	bool mouseOver;
	bool button;

	void updatePosition();

	Sprite() {}; ///<기본 생성자는 외부에사 사용할 수 없도록 제한

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