#pragma once
#ifndef __GRAPHIC_ELEMENT__
#define __GRAPHIC_ELEMENT__

#include <SFML\Graphics.hpp>
#include <functional>
#include <memory>
#include <thread>
#include <string>

/**
* �׷��� ������ ���� �ؽ�ó�� shared-pointer�� ������ Ŭ����.
* @date 2018.11.26
* @author �輺��
*/
class GraphicElement
{
private:
	sf::Sprite* sprite; ///<������ �׷����� ����ϴ� ��������Ʈ ������ ����.
	std::shared_ptr<sf::Texture> texture; ///<�ؽ�ó�� �����ϴ� ������ ����. ���� �ؽ�ó�� ���� ��ü�� ������ �� �����Ƿ� shared pointer�� ������.
	sf::Vector2f position; ///<��������Ʈ�� ��ġ ����.

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