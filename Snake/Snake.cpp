#include "Snake.h"

Snake::Snake(Texture* t)
{
	head = t;
}

void Snake::addBody(Texture* t)
{
	Snake* search = this;

	while (search->body != NULL)
	{
		search = search->body;
	}

	search->body = new Snake(t);
}

void Snake::setDest(SDL_Rect dest)
{
	SDL_Rect oldDest = head->getDest();
	head->setDest(dest);
	
	if (body != NULL)
	{
		body->setDest(oldDest);
	}
}

SDL_Rect Snake::getDest()
{
	return head->getDest();
}

SDL_Rect Snake::getSource()
{
	return head->getSource();
}

void Snake::render(SDL_Renderer* gameRenderer)
{	
	Snake* search = this;

	while (search != NULL)
	{
		search->head->render(gameRenderer);
		search = search->body;
	}
}

bool Snake::detectSelfHit(SDL_Rect hitbox)
{
	Snake* search = body;
	
	while (search != NULL)
	{
		if (SDL_HasIntersection(&hitbox, &search->head->getDest()))
		{
			cout << "The snake touched itself" << endl;
			return true;
		}

		search = search->body;
	}

	return false;
}

void Snake::free()
{
	if (body != NULL) body->free();
	head->freeTexture();
}