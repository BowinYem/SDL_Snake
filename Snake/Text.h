#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include "Texture.h"
using namespace std;

class Text : public Texture{

	const char* message;
	TTF_Font* font;
	int fontSize;
	SDL_Color color; 

public:
	Text(char* message, SDL_Color color, char* fontPath, int fontSize, SDL_Renderer* renderer, SDL_Rect source, SDL_Rect dest);
	void setText(const char* message);
	void createText(SDL_Renderer* renderer);
};

#endif