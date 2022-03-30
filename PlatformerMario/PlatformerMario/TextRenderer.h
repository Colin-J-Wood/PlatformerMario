#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "Commons.h"

using namespace std;

class TextRenderer
{
public:
	TextRenderer(SDL_Renderer* renderer);
	~TextRenderer();

	void Render(Vector2D position);
	bool LoadFont(const string filename, int font_size, const string message, SDL_Color color);

private:
	SDL_Renderer* m_renderer;
	SDL_Texture* m_texture;
	mutable SDL_Rect text_rect;
	TTF_Font* font;
};

