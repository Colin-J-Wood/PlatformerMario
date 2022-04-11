#include "TextRenderer.h"

TextRenderer::TextRenderer(SDL_Renderer* renderer, string filename, int fontsize)
{
	m_renderer = renderer;
	font = TTF_OpenFont(filename.c_str(), fontsize);
	if (font == nullptr)
	{
		cout << "Error loading font: " << filename << endl;
	}
}

TextRenderer::~TextRenderer()
{
	TTF_CloseFont(font);
}

void TextRenderer::Render(Vector2D position)
{
	text_rect.x = position.x;
	text_rect.y = position.y;

	if (m_texture != nullptr) SDL_RenderCopy(m_renderer, m_texture, nullptr, &text_rect);
}

//returns false if it failed.
bool TextRenderer::LoadString(const string message, SDL_Color color)
{
	if (font != nullptr)
	{
		SDL_DestroyTexture(m_texture);

		SDL_Surface* text_surface = TTF_RenderText_Solid(font, message.c_str(), color);
		if (text_surface == nullptr)
		{
			cout << "Error getting font surface!" << endl;
			return false;
		}
		else
		{
			m_texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);

			SDL_FreeSurface(text_surface);
			
			SDL_QueryTexture(m_texture, nullptr, nullptr, &text_rect.w, &text_rect.h);
			return true;
		}
	}
	else
	{
		return false;
	}
	return true;
}