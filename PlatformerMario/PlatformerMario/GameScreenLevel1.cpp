#include "GameScreenLevel1.h"
#include "Texture2D.h"
#include "Character.h"
#include "Collisions.h"

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
	mario->Update(deltaTime, e, m_levelmap);
	luigi->Update(deltaTime, e, m_levelmap);

	if (Collisions::Instance()->Circle(mario->GetPosition(),luigi->GetPosition(),mario->GetCollisionRadius(), luigi->GetCollisionRadius()))
	{
		cout << "Circle hit!" << endl;
	}

	if (Collisions::Instance()->Box(mario->GetCollisionBox(), luigi->GetCollisionBox()))
	{
		cout << "Box hit!" << endl;
	}
}

bool GameScreenLevel1::SetUpLevel()
{
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/level/BackgroundMB.png"))
	{
		cout << "Failed to load background texture!" << endl;
		return false;
	}

	//set up the player character
	mario = new Mario(m_renderer, "Images/entity/Mario.png", Vector2D(64, 330));
	luigi = new Luigi(m_renderer, "Images/entity/Luigi.png", Vector2D(256, 330));

	m_levelmap = new LevelMap("Maps/level1.txt",DEFAULT_TILESIZE);

	return true;
}