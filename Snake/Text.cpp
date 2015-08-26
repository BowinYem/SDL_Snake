#include "Text.h"

Text::Text(char* message, SDL_Color color, char* fontPath, int fontSize, SDL_Renderer* renderer, SDL_Rect source, SDL_Rect dest)
{
	Text::message = message;
	Text::color = color;
	font = TTF_OpenFont(fontPath, fontSize);
	Text::fontSize = fontSize;
	Text::source = source;
	Text::dest = dest;
	createText(renderer);
}

void Text::setText(const char* message)
{
	Text::message = message;
}

void Text::createText(SDL_Renderer* renderer)
{
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, message, color);
	texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
}