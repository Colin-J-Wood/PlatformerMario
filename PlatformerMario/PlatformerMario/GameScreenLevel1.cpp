#include "GameScreenLevel1.h"
#include "Texture2D.h"
#include "Character.h"

using namespace std;

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpLevel();
}

GameScreenLevel1::~GameScreenLevel1()
{
	delete(m_background_texture);
	delete(mario);
	delete(luigi);
	m_background_texture = nullptr;
}

void GameScreenLevel1::Render()
{
	m_background_texture->Render(Vector2D(), SDL_FLIP_NONE);

	mario->Render();
	luigi->Render();
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	mario->Update(deltaTime, e);
	luigi->Update(deltaTime, e);
}

bool GameScreenLevel1::SetUpLevel()
{
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/test.bmp"))
	{
		cout << "Failed to load background texture!" << endl;
		return false;
	}

	//set up the player character
	mario = new Mario(m_renderer, "Images/entity/Mario.png", Vector2D(64, 330));
	luigi = new Luigi(m_renderer, "Images/entity/Luigi.png", Vector2D(256, 330));

	return true;
}