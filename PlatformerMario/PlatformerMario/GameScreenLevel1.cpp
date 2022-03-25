#include "GameScreenLevel1.h"
#include "Texture2D.h"
#include "Character.h"
#include "Collisions.h"
#include <random>

using namespace std;

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpLevel();

	m_music = Mix_LoadMUS("Music/MarioUnderworld.mp3");
	if (m_music == nullptr)
	{
		cout << "Failed to load music.  Error: " << Mix_GetError() << endl;
	}

	if (Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(m_music, -1);
	}

	m_kill_koopa = new Sound("Sound/contact_kill.mp3");
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
	mario->Render();
	luigi->Render();

	m_powBlock->Render();

	//render every koopa using foreach.
	for (Koopa* koopa : m_enemies)
	{
		koopa->Render();
	}

	//render all tiles in front of entities so pipes can hide koopas.
	m_background_texture->Render(Vector2D(0, m_background_yPos), SDL_FLIP_NONE);
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	mario->Update(deltaTime, e, m_levelmap);
	luigi->Update(deltaTime, e, m_levelmap);

	//add a new enemy if the list has less.
	if (m_enemies.size() < 2)
	{
		//respawn on a timer
		m_respawn_time += deltaTime;
		if (m_respawn_time > KOOPA_RESPAWN_TIME)
		{
			//spawn at randomly left or right top pipe
			int rand_result = rand() % 2;
			if (rand_result == 1)
			{
				CreateKoopa(Vector2D(75, 32), FACING_RIGHT, KOOPA_SPEED);
			}
			else
			{
				CreateKoopa(Vector2D(400, 32), FACING_LEFT, KOOPA_SPEED);
			}

			//then reset the timer for the next enemy.
			m_respawn_time = 0.0f;
		}
	}

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

	//teleport all characters to the other side of the screen if their centre crosses the boundary
	if (mario->GetCenterPosition().x < 0)
	{
		mario->SetPosition(Vector2D(SCREEN_WIDTH - mario->GetWidth(), mario->GetPosition().y));
	}
	else if (mario->GetCenterPosition().x > SCREEN_WIDTH)
	{
		mario->SetPosition(Vector2D(0, mario->GetPosition().y));
	}

	if (luigi->GetCenterPosition().x < 0)
	{
		luigi->SetPosition(Vector2D(SCREEN_WIDTH - luigi->GetWidth(), luigi->GetPosition().y));
	}
	else if (luigi->GetCenterPosition().x > SCREEN_WIDTH)
	{
		luigi->SetPosition(Vector2D(0, luigi->GetPosition().y));
	}
	if (!m_enemies.empty())
	{
		for (int i = 0; i < m_enemies.size(); i++)
		{
			if (m_enemies[i]->GetCenterPosition().x < 0)
			{
				m_enemies[i]->SetPosition(Vector2D(SCREEN_WIDTH - m_enemies[i]->GetWidth(), m_enemies[i]->GetPosition().y));
			}
			else if (m_enemies[i]->GetCenterPosition().x > SCREEN_WIDTH)
			{
				m_enemies[i]->SetPosition(Vector2D(0, m_enemies[i]->GetPosition().y));
			}
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

	CreateKoopa(Vector2D(75, 32), FACING_RIGHT, KOOPA_SPEED);
	CreateKoopa(Vector2D(400, 32), FACING_LEFT, KOOPA_SPEED);

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
				if (m_enemies[i]->GetPosition().x < 64.0f - m_enemies[i]->GetWidth() || 
					m_enemies[i]->GetPosition().x > SCREEN_WIDTH - 96.0f + m_enemies[i]->GetWidth()) 
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
					//this collision check is somehow failing, debug it.


					if (m_enemies[i]->GetInjured())
					{
						m_kill_koopa->PlaySound(0);
						m_enemies[i]->SetAlive(false);
					}
					else
					{
						//kill mario
					}
				}

				//don't do this if mario is already dying
				if (Collisions::Instance()->Circle(m_enemies[i]->GetPosition(), luigi->GetPosition(), m_enemies[i]->GetCollisionRadius(), luigi->GetCollisionRadius()) &&
					luigi->GetAlive())
				{
					if (m_enemies[i]->GetInjured())
					{
						m_kill_koopa->PlaySound(0);
						m_enemies[i]->SetAlive(false);
					}
					else
					{
						//kill mario
					}
				}
			}

			//if the enemy is no longer alive, then schedule it for deletion
			if (!m_enemies[i]->GetAlive())
			{
				enemyIndexToDelete = i;
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