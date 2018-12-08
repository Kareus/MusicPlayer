#include "Group.h"

Group::Group()
{
	position.x = 0;
	position.y = 0;
	viewRect.left = -1;
	drawings.SetCompareFunction(compareGraphics);
	drawings.MakeEmpty();
	focus = nullptr;
}

Group::~Group()
{
	MakeEmpty();
}

int Group::AddGraphic(Graphic* g, bool inherit)
{
	g->setID(drawings.GetLength());
	sf::Vector2f p = g->GetPosition();
	g->SetPosition(position.x + p.x, position.y + p.y);
	if (inherit) g->SetData(data);
	return drawings.Add(g);
}

int Group::DeleteGraphic(Graphic* g)
{
	return drawings.Delete(g);
}

void Group::MakeEmpty(bool clear)
{
	focus = nullptr;

	if (clear) //리스트의 메모리 해제 여부
	{
		DoublyIterator<Graphic*> iter(drawings);
		iter.ResetToLastPointer();
		Graphic* g;

		while (iter.NotNull())
		{
			g = iter.Current();

			if (g != nullptr) delete g;
			g = nullptr;
			iter.Prev();
		}
	}
	

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

	bool clip = false;
	if (viewRect.left >= 0) clip = true;

	while (iter.NotNull())
	{
		g = iter.Current();
		if (clip)
		{
			glEnable(GL_SCISSOR_TEST);
			glScissor(viewRect.left + position.x, window->getSize().y - viewRect.height - viewRect.top - position.y, viewRect.width, viewRect.height);
			g->draw(window);
			glDisable(GL_SCISSOR_TEST);
		}
		else g->draw(window);
		iter.Next();
	}
}

void Group::SetPosition(float x, float y)
{
	DoublyIterator<Graphic*> iter(drawings);
	Graphic* g;
	sf::Vector2f pos;

	while (iter.NotNull())
	{
		g = iter.Current();
		pos = g->GetPosition();
		g->SetPosition(pos.x + x - position.x, pos.y + y - position.y);
		iter.Next();
	}

	position.x = x;
	position.y = y;
}

void Group::SetPositionX(float x)
{
	DoublyIterator<Graphic*> iter(drawings);
	Graphic* g;
	sf::Vector2f pos;

	while (iter.NotNull())
	{
		g = iter.Current();
		pos = g->GetPosition();
		g->SetPositionX(pos.x + x - position.x);
		iter.Next();
	}


	position.x = x;
}

void Group::SetPositionY(float y)
{
	DoublyIterator<Graphic*> iter(drawings);
	Graphic* g;
	sf::Vector2f pos;

	while (iter.NotNull())
	{
		g = iter.Current();
		pos = g->GetPosition();
		g->SetPositionY(pos.y + y - position.y);
		iter.Next();
	}

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
		custom.type = CustomWinEvent::MouseOver;
		custom.mouse = CustomWinEvent::MouseEvent();
		custom.mouse.x = e.mouse.x;
		custom.mouse.y = e.mouse.y;

		while (iter.NotNull())
		{
			g = iter.Current();

			if (g->hasPoint(sf::Vector2f(e.mouse.x, e.mouse.y))) g->pollEvent(custom);
			else g->pollEvent(e);

			iter.Prev();
		}
		break;

	case CustomWinEvent::MouseLeft:
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

	if (viewRect.left < 0) return position.x <= point.x && point.x < position.x + size.x && position.y <= point.y && point.y < position.y + size.y;

	if (size.x > viewRect.width) size.x = viewRect.width;
	if (size.y > viewRect.height) size.y = viewRect.height;

	return position.x + viewRect.left <= point.x && point.x < position.x + viewRect.left + size.x &&
		position.y + viewRect.top <= point.y && point.y < position.y + viewRect.top + size.y;
}

DoublyIterator<Graphic*> Group::GetIterator()
{
	DoublyIterator<Graphic*> iter(drawings);
	return iter;
}

void Group::SetViewRect(float x, float y, float width, float height)
{
	viewRect.left = x;
	viewRect.top = y;
	viewRect.width = width;
	viewRect.height = height;
}

sf::FloatRect Group::GetViewRect()
{
	return viewRect;
}