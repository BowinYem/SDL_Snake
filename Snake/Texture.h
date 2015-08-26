#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
using namespace std;

class Texture
{
protected:
	SDL_Texture* texture;
	SDL_Rect source;
	SDL_Rect dest;

public:
	Texture();
	Texture(char* path, SDL_Renderer* renderer);
	Texture(char* path, SDL_Renderer* renderer, SDL_Rect source, SDL_Rect dest);
	~Texture();
	void render(SDL_Renderer* renderer);
	void setSource(SDL_Rect source);
	void setDest(SDL_Rect dest);
	SDL_Rect getSource();
	SDL_Rect getDest();
	void freeTexture();
};

#endif 