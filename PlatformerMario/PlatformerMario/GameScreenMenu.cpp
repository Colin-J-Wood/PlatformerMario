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
	m_high_scores_button = new TextRenderer(renderer, "Fonts/kongtext.ttf", 15);
}

GameScreenMenu::~GameScreenMenu()
{

}

void GameScreenMenu::Render()
{
	m_logo->Render(Vector2D(100, 100), SDL_FLIP_NONE, 0.0);

	m_level_1_button->Render(Vector2D(150, 300));
	m_level_2_button->Render(Vector2D(150, 320));
	m_high_scores_button->Render(Vector2D(150, 340));
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
			if (m_selection > 2) m_selection = 0;

			break;

		case SDLK_UP:
			m_selection--;
			if (m_selection < 0) m_selection = 2;

			break;

		case SDLK_RETURN:
			switch (m_selection)
			{
			case 0:
				return SCREEN_LEVEL1;
				break;

			case 1:
				//screen level 2
				break;

			case 2:
				//screen high scores
				break;

			}
		}
	}

	switch (m_selection)
	{
	case 0:
		m_level_1_button->LoadString("> Level 1", { 255, 255, 255, 255 });
		m_level_2_button->LoadString("  Level 2", { 255, 255, 255, 255 });
		m_high_scores_button->LoadString("  High Scores", { 255, 255, 255, 255 });
		break;
		
	case 1:
		m_level_1_button->LoadString("  Level 1", { 255, 255, 255, 255 });
		m_level_2_button->LoadString("> Level 2", { 255, 255, 255, 255 });
		m_high_scores_button->LoadString("  High Scores", { 255, 255, 255, 255 });
		break;

	case 2:
		m_level_1_button->LoadString("  Level 1", { 255, 255, 255, 255 });
		m_level_2_button->LoadString("  Level 2", { 255, 255, 255, 255 });
		m_high_scores_button->LoadString("> High Scores", { 255, 255, 255, 255 });
		break;
	}


	return SCREEN_MENU;
}