#include "Texture.h"

Texture::Texture(){}

Texture::Texture(char* path, SDL_Renderer* renderer)
{
	SDL_Surface* surface = IMG_Load(path);

	if (surface == NULL)
	{
		cout << "IMG_LOAD FAILED";
	}

	if (SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF)) == -1)
	{
		cout << "SDL_SETCOLORKEY FAILED";
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);
}

Texture::Texture(char* path, SDL_Renderer* renderer, SDL_Rect source, SDL_Rect dest)
{
	SDL_Surface* surface = IMG_Load(path);
	SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF));
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	Texture::source = source;
	Texture::dest = dest;
}

Texture::~Texture()
{
	SDL_DestroyTexture(texture);
}

void Texture::render(SDL_Renderer* renderer)
{
	if (SDL_RenderCopy(renderer, texture, &source, &dest) == -1)
	{
		cout << "SDL_RENDERCOPY FAILED";
	}
}

void Texture::setSource(SDL_Rect source)
{
	Texture::source = source;
}

void Texture::setDest(SDL_Rect dest)
{
	Texture::dest = dest;
}

SDL_Rect Texture::getSource()
{
	return source;
}

SDL_Rect Texture::getDest()
{
	return dest;
}

void Texture::freeTexture()
{
	SDL_DestroyTexture(texture);
}

