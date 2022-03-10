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
	delete(m_levelmap);
	delete(m_powBlock);
	m_background_texture = nullptr;
	m_powBlock = nullptr;
}

void GameScreenLevel1::Render()
{
	m_background_texture->Render(Vector2D(), SDL_FLIP_NONE);

	mario->Render();
	luigi->Render();

	m_powBlock->Render();
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

	if (Collisions::Instance()->Box(mario->GetCollisionBox(), m_powBlock->GetCollisionBox()))
	{
		if ((mario->GetVelocity().y > 0.0f) && m_powBlock->isAvailable())
		{
			m_powBlock->TakeHit();

			//set mario's velocity so he falls from the block, then correct his positioning.
			mario->SetVelocity(Vector2D(mario->GetVelocity().x, 0.0f));
			mario->SetPosition(Vector2D(mario->GetPosition().x, m_powBlock->GetPosition().y + m_powBlock->GetHeight()));
		}
	}

	if (Collisions::Instance()->Box(luigi->GetCollisionBox(), m_powBlock->GetCollisionBox()))
	{
		if ((luigi->GetVelocity().y > 0.0f) && m_powBlock->isAvailable())
		{
			m_powBlock->TakeHit();

			//set mario's velocity so he falls from the block, then correct his positioning.
			luigi->SetVelocity(Vector2D(luigi->GetVelocity().x, 0.0f));
			luigi->SetPosition(Vector2D(luigi->GetPosition().x, m_powBlock->GetPosition().y + m_powBlock->GetHeight()));
		}
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
	m_powBlock = new POWBlock(m_renderer, m_levelmap);

	return true;
}