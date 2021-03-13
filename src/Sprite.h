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
* @author Kareus
*/
class Sprite : public Graphic
{
private:
	sf::Sprite* sprite; ///<실제로 그래픽을 출력하는 스프라이트 포인터 변수.
	std::shared_ptr<sf::Texture> texture; ///<텍스처를 보관하는 포인터 변수. 같은 텍스처를 여러 객체가 공유할 수 있으므로 shared pointer로 선언함.
	sf::Vector2f position; ///<스프라이트의 위치 변수.
	std::function<void(Sprite*)> mouseDownFunc; ///<마우스를 눌렀을 때 실행할 함수
	std::function<void(Sprite*)> mouseUpFunc; ///<마우스를 뗐을 때 실행할 함수
	sf::Color overColor; ///<마우스가 위에 있을 때 표시할 색깔
	sf::Color normalColor; ///<일반적인 표시 색깔
	sf::IntRect textureRect; ///<텍스처에서 가져올 영역 크기
	bool mouseOver; ///<마우스 오버 여부
	bool button; ///<버튼 여부

	/**
	*	@brief	위치를 조정하는 함수
	*/
	void updatePosition();

	/**
	*	기본 생성자
	*/
	Sprite() {}; ///<기본 생성자는 외부에사 사용할 수 없도록 제한 (sprite 및 texture 관리를 위함)

public:
	/**
	*	@brief	해당 경로의 이미지 파일을 읽어와 그래픽을 생성하는 생성자
	*	@param	texturePath	텍스처 경로
	*/
	Sprite(const std::string& texturePath);

	/**
	*	복사 생성자
	*/
	Sprite(const Sprite& data);

	/**
	*	기본 소멸자
	*/
	~Sprite();

	/**
	*	@brief	윈도우에 그래픽을 렌더링하는 함수
	*	@param	window	렌더링할 윈도우
	*/
	void draw(sf::RenderWindow* window) override;

	/**
	*	@brief	그래픽의 위치를 지정하는 함수
	*	@param	x	x좌표
	*	@param	y	y좌표
	*/
	void SetPosition(float x, float y) override;

	/**
	*	@brief	그래픽의 위치를 지정하는 함수
	*	@param	x	x좌표
	*/
	void SetPositionX(float x) override;

	/**
	*	@brief	그래픽의 위치를 지정하는 함수
	*	@param	y	y좌표
	*/
	void SetPositionY(float y) override;

	/**
	*	@brief	그래픽의 텍스처 크기를 지정하는 함수
	*	@param	width	너비
	*	@param	height	폭
	*/
	void SetTextureRect(float width, float height);

	/**
	*	@brief	그래픽의 텍스처 위치를 지정하는 함수
	*	@param	x	x좌표
	*	@param	y	y좌표
	*/
	void SetTexturePos(float x, float y);

	/**
	*	@brief	그래픽의 위치를 반환하는 함수
	*	@return	그래픽의 위치.
	*/
	sf::Vector2f GetPosition() override;

	/**
	*	@brief	그래픽의 텍스처 위치를 반환하는 함수
	*	@return	그래픽의 텍스처 위치.
	*/
	sf::Vector2f GetTexturePos();

	/**
	*	@brief	그래픽의 크기를 반환하는 함수
	*	@return	그래픽의 크기.
	*/
	sf::Vector2f GetSize() override;

	/**
	*	@brief	그래픽의 버튼 여부를 지정하는 함수
	*	@param	b	버튼 여부
	*/
	void SetButton(bool b);

	/**
	*	@brief	그래픽이 버튼인지 여부를 반환한다.
	*	@return	버튼이면 true, 아니면 false를 반환한다.
	*/
	bool isButton() const;

	/**
	*	@brief	그래픽 위에서 마우스를 눌렀을 때 실행할 함수를 지정한다.
	*	@param	func	실행할 함수
	*/
	void SetMouseDownFunction(const std::function<void(Sprite*)>& func);

	/**
	*	@brief	그래픽 위에서 마우스를 뗐을 때 실행할 함수를 지정한다.
	*	@param	func	실행할 함수
	*/
	void SetMouseUpFunction(const std::function<void(Sprite*)>& func);

	/**
	*	@brief	이벤트를 받아 각 그래픽에서 실행하는 함수
	*	@param	e	이벤트
	*	@return	이벤트를 성공적으로 처리했는지 여부가 반환된다
	*/
	bool pollEvent(CustomWinEvent e) override;

	/**
	*	@brief	입력받은 점이 현재 그래픽의 영역에 포함되는지 여부를 반환한다.
	*	@param	point	좌표
	*	@return	포함되면 true, 아니면 false를 반환한다.
	*/
	bool hasPoint(const sf::Vector2f& point) override;

	/**
	*	@brief	마우스 오버 상태를 리셋한다.
	*/
	void ResetMouseOver();

	/**
	*	@brief	마우스 오버 이벤트를 실행한다.
	*/
	void TriggerMouseOver();

	/**
	*	@brief	그래픽의 복사본을 반환한다.
	*	@return	그래픽의 복사본
	*/
	Sprite* clone();
};
#endif