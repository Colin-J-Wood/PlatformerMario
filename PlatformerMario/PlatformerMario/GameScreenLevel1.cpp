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

	m_enemies.clear();

	m_background_texture = nullptr;
	m_powBlock = nullptr;
}

void GameScreenLevel1::Render()
{
	m_background_texture->Render(Vector2D(0, m_background_yPos), SDL_FLIP_NONE);

	mario->Render();
	luigi->Render();

	m_powBlock->Render();

	//render every koopa using foreach.
	for (Koopa* koopa : m_enemies)
	{
		koopa->Render();
	}
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	mario->Update(deltaTime, e, m_levelmap);
	luigi->Update(deltaTime, e, m_levelmap);

	UpdateEnemies(deltaTime, e, m_levelmap);

	//collide with pow block
	if (Collisions::Instance()->Box(mario->GetCollisionBox(), m_powBlock->GetCollisionBox()))
	{
		//only process this collision if the player was rising into the block
		if ((mario->GetVelocity().y < 0.0f) && m_powBlock->isAvailable())
		{
			m_powBlock->TakeHit();
			DoScreenshake(deltaTime);

			//set mario's velocity so he falls from the block, then correct his positioning.
			mario->SetVelocity(Vector2D(mario->GetVelocity().x, 0.0f));
			mario->SetPosition(Vector2D(mario->GetPosition().x, m_powBlock->GetPosition().y + m_powBlock->GetHeight()));
		}
	}

	if (Collisions::Instance()->Box(luigi->GetCollisionBox(), m_powBlock->GetCollisionBox()))
	{
		//only process this collision if the player was rising into the block
		if ((luigi->GetVelocity().y < 0.0f) && m_powBlock->isAvailable())
		{
			m_powBlock->TakeHit();
			DoScreenshake(deltaTime);

			//set mario's velocity so he falls from the block, then correct his positioning.
			luigi->SetVelocity(Vector2D(luigi->GetVelocity().x, 0.0f));
			luigi->SetPosition(Vector2D(luigi->GetPosition().x, m_powBlock->GetPosition().y + m_powBlock->GetHeight()));
		}
	}

	//do the screenshake if required.
	if (m_screenshake)
	{
		m_shake_time -= deltaTime;
		m_wobble++;
		m_background_yPos = sin(m_wobble);
		m_background_yPos *= 3.0f;

		//end shake after duration
		if (m_shake_time <= 0.0f)
		{
			m_screenshake = false;
			m_background_yPos = 0.0f;
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

	CreateKoopa(Vector2D(150, 32), FACING_RIGHT, KOOPA_SPEED);
	CreateKoopa(Vector2D(325, 32), FACING_LEFT, KOOPA_SPEED);

	return true;
}

void GameScreenLevel1::DoScreenshake(float deltaTime)
{
	m_screenshake = true;
	m_shake_time = SHAKE_DURATION;
	m_wobble = 0.0f;

	//make all enemies take damage
	for (Koopa* koopa : m_enemies)
	{
		koopa->TakeDamage(deltaTime);
	}
}

void GameScreenLevel1::UpdateEnemies(float deltaTime, SDL_Event e, LevelMap* map)
{
	if (!m_enemies.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < m_enemies.size(); i++)
		{
			//check if the enemy is on the bottom row
			if (m_enemies[i]->GetPosition().y > 300.0f)
			{
				//is the enemy off screen?
				if (m_enemies[i]->GetPosition().x < (float)(-m_enemies[i]->GetCollisionBox().width * 0.5f) || 
					m_enemies[i]->GetPosition().x > SCREEN_WIDTH - (float)(m_enemies[i]->GetCollisionBox().width * 0.55f)) 
					m_enemies[i]->SetAlive(false);
			}
			//now do the update

			m_enemies[i]->Update(deltaTime, e, map);

			//check to see if enemy collides with player
			if ((m_enemies[i]->GetPosition().y > 300.0f || 
				m_enemies[i]->GetPosition().y <= 64.0f) && 
				(m_enemies[i]->GetPosition().x < 64.0f || 
				m_enemies[i]->GetPosition().x > SCREEN_WIDTH - 96.0f))
			{
				//ignore collisions if behind a pipe
			}
			else
			{
				//don't do this if mario is already dying
				if (Collisions::Instance()->Circle(m_enemies[i]->GetPosition(), mario->GetPosition(), m_enemies[i]->GetCollisionRadius(), mario->GetCollisionRadius()) &&
					mario->GetAlive())
				{
					if (m_enemies[i]->GetInjured())
					{
						m_enemies[i]->SetAlive(false);
					}
					else
					{
						//kill mario
					}

					//if the enemy is no longer alive, then schedule it for deletion
					if (!m_enemies[i]->GetAlive())
					{
						enemyIndexToDelete = i;
					}
				}
			}
		}

		//remove dead enemies -1 each update

		if (enemyIndexToDelete != -1)
		{
			m_enemies.erase(m_enemies.begin() + enemyIndexToDelete);
		}
	}
}

void GameScreenLevel1::CreateKoopa(Vector2D position, FACING direction, Vector2D target_speed)
{
	Koopa* koopa = new Koopa(m_renderer, "Images/entity/Koopa.png", m_levelmap, position, direction, target_speed);
	m_enemies.push_back(koopa);
}