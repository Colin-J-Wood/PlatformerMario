#include "GameScreenManager.h"
#include "GameScreenLevel1.h"
#include "GameScreenMenu.h"

using namespace std;

GameScreenManager::GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen)
{
	m_renderer = renderer;
	m_current_screen = nullptr;

	m_screen_selected = startScreen;

	ChangeScreen(startScreen);
}

GameScreenManager::~GameScreenManager()
{
	m_renderer = nullptr;

	delete(m_current_screen);
	m_current_screen = nullptr;
}

void GameScreenManager::Render()
{
	m_current_screen->Render();
}

void GameScreenManager::Update(float deltaTime, SDL_Event e)
{
	SCREENS m_next_screen = m_current_screen->Update(deltaTime, e);

	//switch screens if another screen target is called.
	if (m_next_screen != m_screen_selected)
	{
		ChangeScreen(m_next_screen);
		m_screen_selected = m_next_screen;
	}
}

void GameScreenManager::ChangeScreen(SCREENS new_screen)
{
	if (m_current_screen != nullptr)
	{
		delete m_current_screen;
	}

	GameScreenMenu* menuScreen;
	GameScreenLevel1* tempScreen;

	switch (new_screen)
	{
	case SCREEN_MENU:
		menuScreen = new GameScreenMenu(m_renderer);
		m_current_screen = (GameScreen*)menuScreen;
		menuScreen = nullptr;
		break;

	case SCREEN_LEVEL1:
		tempScreen = new GameScreenLevel1(m_renderer);
		m_current_screen = (GameScreen*)tempScreen;
		tempScreen = nullptr;
		break;

	default:;
	}
}