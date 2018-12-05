#include "Group.h"

Group::Group()
{
	position.x = 0;
	position.y = 0;
	drawings.SetCompareFunction(compareGraphics);
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
	g->setID(drawings.GetLength());
	g->SetData(data);
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
	if (drawings.IsEmpty()) return sf::Vector2f();

	DoublyIterator<Graphic*> iter(drawings);
	Graphic* g;
	sf::Vector2f pos = iter.Current()->GetPosition();
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

bool Group::pollEvent(CustomWinEvent e)
{
	DoublyIterator<Graphic*> iter(drawings);
	iter.ResetToLastPointer();
	Graphic* g;
	CustomWinEvent custom;

	switch (e.type)
	{
	case CustomWinEvent::MouseDown:

		if (focus)
		{
			focus->setFocus(false);
			focus = nullptr;
		}

		while (iter.NotNull())
		{
			g = iter.Current();

			if (g->hasPoint(sf::Vector2f(e.mouse.x, e.mouse.y)))
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

	case CustomWinEvent::MouseMoved:
		while (iter.NotNull())
		{
			g = iter.Current();

			if (g->hasPoint(sf::Vector2f(e.mouse.x, e.mouse.y)))
			{
				custom.type = CustomWinEvent::MouseOver;
				custom.mouse = CustomWinEvent::MouseEvent();
				custom.mouse.x = e.mouse.x;
				custom.mouse.y = e.mouse.y;
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

	case CustomWinEvent::MouseOver:
		
		custom.type = CustomWinEvent::MouseMoved;
		custom.mouse = CustomWinEvent::MouseEvent();
		custom.mouse.x = e.mouse.x;
		custom.mouse.y = e.mouse.y;

		while (iter.NotNull())
		{
			g = iter.Current();

			if (g->hasPoint(sf::Vector2f(e.mouse.x, e.mouse.y))) g->pollEvent(e);
			else g->pollEvent(custom);

			iter.Prev();
		}
		break;

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

void Group::SetData(const std::wstring& data)
{
	this->data = data;
}

std::wstring Group::GetData()
{
	return data;
}