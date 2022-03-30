#include "GameScreen.h"

using namespace std;

GameScreen::GameScreen(SDL_Renderer* renderer)
{
	m_renderer = renderer;
}

GameScreen::GameScreen()
{

}

GameScreen::~GameScreen()
{
	m_renderer = nullptr;
}

void GameScreen::Render() {}
SCREENS GameScreen::Update(float deltaTime, SDL_Event e) { return SCREEN_MENU; }