#include "Texture2D.h"

#include <SDL_image.h>
#include <iostream>

using namespace std;

Texture2D::Texture2D(SDL_Renderer* renderer)
{
	m_renderer = renderer;
	m_width = 0;
	m_height = 0;
	m_texture = nullptr;
}

Texture2D::~Texture2D()
{
	Free();

	m_renderer = nullptr;
}

bool Texture2D::LoadFromFile(std::string path)
{
	Free();

	//load the image
	SDL_Surface* p_surface = IMG_Load(path.c_str());
	if (p_surface != nullptr)
	{
		//colour key the image to be transparent.
		SDL_SetColorKey(p_surface, SDL_TRUE, SDL_MapRGB(p_surface->format, 0, 0xFF, 0xFF));

		//create the texture from the pixels on the surface
		m_texture = SDL_CreateTextureFromSurface(m_renderer, p_surface);
		if (m_texture == nullptr)
		{
			cout << "Unable to create texture from surface. Error: " << SDL_GetError();
			return false;
		}
		else
		{
			m_width = p_surface->w;
			m_height = p_surface->h;
		}
		//remove the loaded surface now that we have a texture.
		SDL_FreeSurface(p_surface);
	}
	else
	{
		cout << "Unable to create surface.  Error: " << SDL_GetError();
	}

	return true;
}

void Texture2D::Free()
{
	if (m_texture == nullptr)
	{
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
	}
}

void Texture2D::Render(Vector2D new_position, SDL_RendererFlip flip, double angle)
{
	//set where to render the texture with a rectangle area the size of screen width and height
	SDL_Rect renderLocation = { new_position.x, new_position.y, m_width, m_height };

	//render the texture but flipped vertically
	SDL_RenderCopyEx(m_renderer, m_texture, nullptr, &renderLocation, angle, nullptr, flip);
}