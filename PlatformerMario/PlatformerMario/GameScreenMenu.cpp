#include "GameScreenMenu.h"

using namespace std;

GameScreenMenu::GameScreenMenu(SDL_Renderer* renderer) : GameScreen(renderer)
{
	m_logo = new Texture2D(renderer);

	if (!m_logo->LoadFromFile("Images/logo.jpg"))
	{
		cout << "Error loading logo from file!" << endl;
	}

	m_level_1_button = new TextRenderer(renderer, "Fonts/kongtext.ttf", 15);
	m_level_2_button = new TextRenderer(renderer, "Fonts/kongtext.ttf", 15);
}

GameScreenMenu::~GameScreenMenu()
{
	delete(m_logo);
	delete(m_level_1_button);
	delete(m_level_2_button);
}

void GameScreenMenu::Render()
{
	m_logo->Render(Vector2D(100, 100), SDL_FLIP_NONE, 0.0);

	m_level_1_button->Render(Vector2D(150, 300));
	m_level_2_button->Render(Vector2D(150, 320));
}

SCREENS GameScreenMenu::Update(float deltaTime, SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_DOWN:
			m_selection++;
			if (m_selection > 1) m_selection = 0;

			break;

		case SDLK_UP:
			m_selection--;
			if (m_selection < 0) m_selection = 1;

			break;

		case SDLK_RETURN:
			switch (m_selection)
			{
			case 0:
				return SCREEN_LEVEL1;
				break;

			case 1:
				//screen level 2
				return SCREEN_LEVEL2;
				break;

			}
		}
	}

	switch (m_selection)
	{
	case 0:
		m_level_1_button->LoadString("> Level 1", { 255, 255, 255, 255 });
		m_level_2_button->LoadString("  Level 2", { 255, 255, 255, 255 });
		break;
		
	case 1:
		m_level_1_button->LoadString("  Level 1", { 255, 255, 255, 255 });
		m_level_2_button->LoadString("> Level 2", { 255, 255, 255, 255 });
		break;
	}


	return SCREEN_MENU;
}