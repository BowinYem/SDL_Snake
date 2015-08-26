#ifndef SNAKE_H
#define SNAKE_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Texture.h"
using namespace std;

class Snake
{
	Texture* head;
	Snake* body;

public:
	Snake(Texture* t);
	void free();
	void addBody(Texture* t);
	void setDest(SDL_Rect dest);
	SDL_Rect getDest();
	SDL_Rect getSource();
	void render(SDL_Renderer* gameRenderer);
	bool detectSelfHit(SDL_Rect hitbox);
};

#endif 