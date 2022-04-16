#include "GameScreenLevel1.h"
#include "Texture2D.h"
#include "Character.h"
#include "Collisions.h"

using namespace std;

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpLevel();

	m_music = Mix_LoadMUS("Music/MarioUnderworld.mp3");
	if (m_music == nullptr)
	{
		cout << "Failed to load music.  Error: " << Mix_GetError() << endl;
	}

	Mix_PlayMusic(m_music, -1);

	m_kill_koopa = new Sound("Sound/contact_kill.mp3");

	m_text_mario_score = new TextRenderer(renderer, "Fonts/kongtext.ttf", 8);
	m_text_luigi_score = new TextRenderer(renderer, "Fonts/kongtext.ttf", 8);
	m_text_mario_lives = new TextRenderer(renderer, "Fonts/kongtext.ttf", 8);
	m_text_luigi_lives = new TextRenderer(renderer, "Fonts/kongtext.ttf", 8);
	m_text_game_over = new TextRenderer(renderer, "Fonts/kongtext.ttf", 15);
}

GameScreenLevel1::~GameScreenLevel1()
{
	delete(m_background_texture);
	delete(mario);
	delete(luigi);
	delete(m_levelmap);
	delete(m_powBlock);

	Mix_FreeMusic(m_music);

	m_enemies.clear();
	m_blocks.clear();

	m_background_texture = nullptr;
	m_powBlock = nullptr;
}

void GameScreenLevel1::Render()
{
	mario->Render();
	luigi->Render();

	//render every block using foreach.
	for (POWBlock* block : m_blocks)
	{
		block->Render();
	}

	//render every koopa using foreach.
	for (Koopa* koopa : m_enemies)
	{
		koopa->Render();
	}

	//render all tiles in front of entities so pipes can hide koopas.
	m_background_texture->Render(Vector2D(0, m_background_yPos), SDL_FLIP_NONE);

	m_text_mario_score->Render(Vector2D(70, 10));
	m_text_luigi_score->Render(Vector2D(260, 10));
	m_text_mario_lives->Render(Vector2D(70, 20));
	m_text_luigi_lives->Render(Vector2D(260, 20));
	m_text_game_over->Render(Vector2D(230, 230));
}

SCREENS GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	mario->Update(deltaTime, e, m_levelmap);
	luigi->Update(deltaTime, e, m_levelmap);

	//add a new enemy if the list has less.
	if (m_enemies.size() < 4)
	{
		//respawn on a timer
		m_respawn_time += deltaTime;
		if (m_respawn_time > KOOPA_RESPAWN_TIME)
		{
			//spawn furthest away from player instead of random
			int side = 0;
			if (mario->GetCenterPosition().x > SCREEN_WIDTH / 2) side += 1;
			if (luigi->GetCenterPosition().x > SCREEN_WIDTH / 2) side += 1;
			if (side == 2)
			{
				CreateKoopa(Vector2D(20, 32), FACING_RIGHT, KOOPA_SPEED);
			}
			else if (side == 0)
			{
				CreateKoopa(Vector2D(460, 32), FACING_LEFT, KOOPA_SPEED);
			}
			else
			{
				//do random because both players are on opposite sides
				if ((rand() % 2 + 1) == 1)
				{
					CreateKoopa(Vector2D(20, 32), FACING_RIGHT, KOOPA_SPEED);
				}
				else
				{
					CreateKoopa(Vector2D(460, 32), FACING_LEFT, KOOPA_SPEED);
				}
			}

			//then reset the timer for the next enemy.
			m_respawn_time = 0.0f;
		}
	}

	UpdateEnemies(deltaTime, e, m_levelmap);

	//collide with pow block
	for (int i = 0; i < m_blocks.size(); i++)
	{
		if (Collisions::Instance()->Box(mario->GetCollisionBox(), m_blocks[i]->GetCollisionBox()))
		{
			//only process this collision if the player was rising into the block
			if ((mario->GetVelocity().y < 0.0f) && m_blocks[i]->isAvailable() && (mario->GetCenterPosition().x > m_blocks[i]->GetPosition().x) && (mario->GetCenterPosition().x < (m_blocks[i]->GetPosition().x + m_blocks[i]->GetWidth())))
			{
				m_blocks[i]->TakeHit();
				m_score_mario += POW_SCORE;
				DoScreenshake(deltaTime);

				//set mario's velocity so he falls from the block, then correct his positioning.
				mario->SetVelocity(Vector2D(mario->GetVelocity().x, 0.0f));
				mario->SetPosition(Vector2D(mario->GetPosition().x, m_blocks[i]->GetPosition().y + m_blocks[i]->GetHeight()));
			}
		}

		if (Collisions::Instance()->Box(luigi->GetCollisionBox(), m_blocks[i]->GetCollisionBox()))
		{
			//only process this collision if the player was rising into the block
			if ((luigi->GetVelocity().y < 0.0f) && m_blocks[i]->isAvailable() && (luigi->GetCenterPosition().x > m_blocks[i]->GetPosition().x) && (luigi->GetCenterPosition().x < (m_blocks[i]->GetPosition().x + m_blocks[i]->GetWidth())))
			{
				m_blocks[i]->TakeHit();
				m_score_luigi += POW_SCORE;
				DoScreenshake(deltaTime);

				//set mario's velocity so he falls from the block, then correct his positioning.
				luigi->SetVelocity(Vector2D(luigi->GetVelocity().x, 0.0f));
				luigi->SetPosition(Vector2D(luigi->GetPosition().x, m_blocks[i]->GetPosition().y + m_blocks[i]->GetHeight()));
			}
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

	//draw all strings
	m_text_mario_score->LoadString("MARIO SCORE: " + to_string(m_score_mario), { 255, 255, 255, 255 });
	m_text_luigi_score->LoadString("LUIGI SCORE: " + to_string(m_score_luigi), { 255, 255, 255, 255 });
	m_text_mario_lives->LoadString("MARIO LIVES: " + to_string(mario->GetLivesRemaining()), { 255, 0, 0, 255 });
	m_text_luigi_lives->LoadString("LUIGI LIVES: " + to_string(luigi->GetLivesRemaining()), { 0, 255, 0, 255 });

	//if the game over has elapsed longer than 8 seconds, go back to main menu.
	if ((mario->GetLivesRemaining() == 0) && (luigi->GetLivesRemaining() == 0))
	{
		m_game_over_time += deltaTime;

		if (m_game_over_time > 3.0f)
		{
			m_text_game_over->LoadString("GAME OVER", { 255, 255, 255, 255 });
		}
		if (m_game_over_time > 8.0f)
		{
			return SCREEN_MENU;
		}
	}

	//stay in the level.
	return SCREEN_LEVEL1;
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
	POWBlock* powBlock1 = new POWBlock(m_renderer, m_levelmap);
	m_blocks.push_back(powBlock1);

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
				(m_enemies[i]->GetPosition().x < 32.0f || 
				m_enemies[i]->GetPosition().x > SCREEN_WIDTH - 64.0f))
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
						m_kill_koopa->PlaySound(0);
						m_enemies[i]->SetAlive(false);

						m_score_mario += KOOPA_SCORE;
					}
					else
					{
						mario->LoseLife();
						mario->SetAlive(false);
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

						m_score_luigi += KOOPA_SCORE;
					}
					else
					{
						luigi->LoseLife();
						luigi->SetAlive(false);
					}
				}

				//allow the players to hit the ceiling and trace a box trace above the ceiling.
				if (mario->GetAlive() && mario->GetCeilingHit())
				{
					//process the translated collision box.
					Rect2D new_box = mario->GetCollisionBox();
					//it should be an entire two tiles above the player.
					new_box.y -= DEFAULT_TILESIZE * 2;

					//do the collision check with the new translated box.
					if (Collisions::Instance()->Box(m_enemies[i]->GetCollisionBox(), new_box) && !m_enemies[i]->GetInjured())
					{
						//damage if not already flipped over.
						m_enemies[i]->TakeDamage(deltaTime);
						m_score_mario += KOOPA_SCORE;
					}
				}

				//allow the players to hit the ceiling and trace a box trace above the ceiling.
				if (luigi->GetAlive() && luigi->GetCeilingHit())
				{
					//process the translated collision box.
					Rect2D new_box = luigi->GetCollisionBox();
					//it should be an entire two tiles above the player.
					new_box.y -= DEFAULT_TILESIZE * 2;

					//do the collision check with the new translated box.
					if (Collisions::Instance()->Box(m_enemies[i]->GetCollisionBox(), new_box) && !m_enemies[i]->GetInjured())
					{
						m_enemies[i]->TakeDamage(deltaTime);
						m_score_luigi += KOOPA_SCORE;
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