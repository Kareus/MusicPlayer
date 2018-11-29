#include "Group.h"

Group::Group()
{
	position.x = 0;
	position.y = 0;
	drawings.MakeEmpty();
	focus = nullptr;
}

Group::~Group()
{
	DoublyIterator<Graphic*> iter(drawings);
	iter.ResetToLastPointer();
	Graphic* g;

	while (iter.NotNull())
	{
		g = iter.Current();

		delete g;
		iter.Prev();
	}

	drawings.MakeEmpty();
}

int Group::AddGraphic(Graphic* g)
{
	return drawings.Add(g);
}

int Group::DeleteGraphic(Graphic* g)
{
	return drawings.Delete(g);
}

void Group::MakeEmpty()
{
	drawings.MakeEmpty();
}

int Group::GetLength() const
{
	return drawings.GetLength();
}

void Group::draw(sf::RenderWindow* window)
{
	DoublyIterator<Graphic*> iter(drawings);
	Graphic* g;

	while (iter.NotNull())
	{
		g = iter.Current();
		g->draw(window);
		iter.Next();
	}
}

void Group::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void Group::SetPositionX(float x)
{
	position.x = x;
}

void Group::SetPositionY(float y)
{
	position.y = y;
}

sf::Vector2f Group::GetPosition()
{
	return position;
}

sf::Vector2f Group::GetSize()
{
	DoublyIterator<Graphic*> iter(drawings);
	Graphic* g;
	sf::Vector2f pos;
	sf::Vector2f size;
	sf::Vector2f temp;

	while (iter.NotNull())
	{
		g = iter.Current();
		temp = g->GetPosition();
		if (pos.x > temp.x) pos.x = temp.x;
		if (pos.y > temp.y) pos.y = temp.y;

		temp += g->GetSize();
		if (size.x < temp.x) size.x = temp.x;
		if (size.y < temp.y) size.y = temp.y;

		iter.Next();
	}

	temp.x = size.x - pos.x;
	temp.y = size.y - pos.y;

	return temp;
}

bool Group::pollEvent(sf::Event e)
{
	//application의 이벤트 처리와 동일함
	CustomWinEvent custom;
	DoublyIterator<Graphic*> iter(drawings);
	iter.ResetToLastPointer();
	Graphic* g;

	switch (e.type)
	{
	case sf::Event::MouseButtonPressed:

		if (focus)
		{
			focus->setFocus(false);
			focus = nullptr;
		}

		while (iter.NotNull())
		{
			g = iter.Current();

			if (g->hasPoint(sf::Vector2f(e.mouseButton.x, e.mouseButton.y)))
			{
				focus = g;
				break;
			}

			iter.Prev();
		}

		if (focus)
		{
			focus->setFocus(true);
			focus->pollEvent(e);
		}
		break;

	case sf::Event::MouseMoved:
		while (iter.NotNull())
		{
			g = iter.Current();

			if (g->hasPoint(sf::Vector2f(e.mouseMove.x, e.mouseMove.y)))
			{
				custom.type = CustomWinEvent::MouseOver;
				custom.mouseOver = CustomWinEvent::MouseOverEvent();
				custom.mouseOver.x = e.mouseMove.x;
				custom.mouseOver.y = e.mouseMove.y;
				g->pollEvent(custom);
				break;
			}
			else g->pollEvent(e);

			iter.Prev();
		}
		break;

	case sf::Event::MouseLeft:
		while (iter.NotNull())
		{
			iter.Current()->pollEvent(e);
			iter.Prev();
		}
		break;

	default:
		if (focus) focus->pollEvent(e);
		break;
	}

	return true;
}

bool Group::pollEvent(CustomWinEvent e)
{
	DoublyIterator<Graphic*> iter(drawings);
	iter.ResetToLastPointer();
	Graphic* g;

	switch (e.type)
	{
	case CustomWinEvent::MouseOver:

		while (iter.NotNull())
		{
			g = iter.Current();

			if (g->hasPoint(sf::Vector2f(e.mouseOver.x, e.mouseOver.y)))
			{
				g->pollEvent(e);
				break;
			}

			iter.Prev();
		}

	default:
		if (focus) focus->pollEvent(e);
		break;
	}

	return true;
}

bool Group::hasPoint(const sf::Vector2f& point)
{
	sf::Vector2f size = GetSize();
	return position.x <= point.x && point.x < position.x + size.x && position.y <= point.y && point.y < position.y + size.y;
}

DoublyIterator<Graphic*> Group::GetIterator()
{
	DoublyIterator<Graphic*> iter(drawings);
	return iter;
}