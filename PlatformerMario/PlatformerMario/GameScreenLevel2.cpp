#include "GameScreenLevel2.h"

GameScreenLevel2::GameScreenLevel2(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpLevel();

	m_music = Mix_LoadMUS("Music/Mario.mp3");
	if (m_music == nullptr)
	{
		cout << "Failed to load music.  Error: " << Mix_GetError() << endl;
	}

	Mix_PlayMusic(m_music, -1);

	m_kill_koopa = new Sound("Sound/contact_kill.mp3");
	m_collect_coin = new Sound("Sound/get_coin.mp3");

	m_text_mario_score = new TextRenderer(renderer, "Fonts/kongtext.ttf", 8);
	m_text_game_over = new TextRenderer(renderer, "Fonts/kongtext.ttf", 15);
}

GameScreenLevel2::~GameScreenLevel2()
{
	delete(m_background_texture);
	delete(mario);
	delete(m_levelmap);
	delete(m_kill_koopa);

	Mix_FreeMusic(m_music);

	m_enemies.clear();
	m_blocks.clear();
	m_collectibles.clear();

	delete(m_text_game_over);
	delete(m_text_mario_score);
}

void GameScreenLevel2::Render()
{
	//render all tiles in front of entities so pipes can hide koopas.
	// SDL_Rect source_rect = SDL_Rect{ 0, 0, m_texture->GetWidth(), m_texture->GetHeight() };
	SDL_Rect source_rect = SDL_Rect{ 0, 0, m_background_texture->GetWidth(), m_background_texture->GetHeight() };
	SDL_Rect destination_rect = SDL_Rect{ (int)camera.x, (int)(camera.y + m_background_yPos), m_background_texture->GetWidth(), m_background_texture->GetHeight() };
	m_background_texture->Render(source_rect, destination_rect, SDL_FLIP_NONE);

	//change renderers to support camera clip.
	mario->Render(camera);

	//render every block using foreach.
	for (POWBlock* block : m_blocks)
	{
		block->Render(camera);
	}

	//render every koopa using foreach.
	for (Koopa* koopa : m_enemies)
	{
		koopa->Render(camera);
	}

	for (Collectible* collectible : m_collectibles)
	{
		collectible->Render(camera);
	}

	m_text_mario_score->Render(Vector2D(70, 10));
	m_text_game_over->Render(Vector2D(230, 230));
}

SCREENS GameScreenLevel2::Update(float deltaTime, SDL_Event e)
{
	mario->Update(deltaTime, e, m_levelmap);

	//center camera to half mario's position
	camera.x = -mario->GetPosition().x + (SCREEN_WIDTH / 2);
	//unless that's less than zero, in such case place it at zero.
	if (camera.x > 0) camera.x = 0;

	//update all the game objects.
	UpdateEnemies(deltaTime, e, m_levelmap);
	UpdateCollectibles(deltaTime, e, m_levelmap);
	UpdateBlocks(deltaTime, e, m_levelmap);

	//make all players stay die off screen
	if (mario->GetPosition().y > m_background_texture->GetHeight())
	{
		//keep mario off screen whilst death occurs.
		mario->SetPosition(Vector2D(mario->GetPosition().x, m_background_texture->GetHeight() + 5));
		mario->SetAlive(false);
		mario->LoseLife();
	}
	//keep mario on screen, only vertical fallout should kill.
	if (mario->GetPosition().x < 0) mario->SetPosition(Vector2D(0, mario->GetPosition().y));

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

	m_text_mario_score->LoadString("MARIO SCORE: " + to_string(m_score_mario), { 255, 255, 255, 255 });

	if (mario->GetLivesRemaining() == 0)
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

	//stay in the level
	return SCREEN_LEVEL2;
}

bool GameScreenLevel2::SetUpLevel()
{
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/level/SuperMarioBrosMap1-1.png"))
	{
		cout << "Failed to load background texture!" << endl;
		return false;
	}

	mario = new Mario(m_renderer, "Images/entity/Mario.png", Vector2D(64, 330));
	mario->SetLivesRemaining(1);

	m_levelmap = new LevelMap("Maps/level2.txt", DEFAULT_TILESIZE);

	SetupBlocks();
	SetupCollectibles();
	SetupEnemies();

	return true;
}

//a trio of simple method blocks to organize the setting up of assets are below
void GameScreenLevel2::SetupBlocks()
{
	POWBlock* test_block = new POWBlock(m_renderer, m_levelmap, "Images/tile/PowBlock.png", "Sound/pow.mp3", 3, POW, Vector2D((SCREEN_WIDTH / 2) - 16, 270));
	m_blocks.push_back(test_block);
	test_block = new POWBlock(m_renderer, m_levelmap, "Images/tile/DestructibleBlock.png", "Sound/destroy.mp3", 1, DESTRUCTIBLE, Vector2D((SCREEN_WIDTH / 2) + 16, 270));
	m_blocks.push_back(test_block);
	test_block = nullptr;
}

void GameScreenLevel2::SetupEnemies()
{
	
}

void GameScreenLevel2::SetupCollectibles()
{
	Collectible* test_coin = new Collectible(m_renderer, "Images/item/Coin.png", m_levelmap, Vector2D(128, 320));
	m_collectibles.push_back(test_coin);
	test_coin = nullptr;
}

void GameScreenLevel2::UpdateEnemies(float deltaTime, SDL_Event e, LevelMap* map)
{
	if (!m_enemies.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < m_enemies.size(); i++)
		{
			//also kill if the enemy has fallen out of the level
			if (m_enemies[i]->GetPosition().y > m_background_texture->GetHeight() + m_enemies[i]->GetHeight())
			{
				enemyIndexToDelete = i;
			}

			//now do the update
			m_enemies[i]->Update(deltaTime, e, map);

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

void GameScreenLevel2::UpdateCollectibles(float deltaTime, SDL_Event e, LevelMap* map)
{
	if (!m_collectibles.empty())
	{
		int collectibleIndexToDelete = -1;
		for (unsigned int i = 0; i < m_collectibles.size(); i++)
		{
			//if collectible is collided with mario...
			if (Collisions::Instance()->Circle(mario->GetCenterPosition(), m_collectibles[i]->GetCenterPosition(), mario->GetCollisionRadius(), m_collectibles[i]->GetCollisionRadius()))
			{
				//collect it.
				switch (m_collectibles[i]->OnCollect())
				{
				case COIN:
					//coin simply gives score.
					m_collect_coin->PlaySound(0);
					m_score_mario += 200;
					
					break;
				case MUSHROOM:
					break;
				}

				collectibleIndexToDelete = i;
			}

			//now do the update.
			m_collectibles[i]->Update(deltaTime, e, map);
		}

		//remove collected items.
		if (collectibleIndexToDelete != -1)
		{
			m_collectibles.erase(m_collectibles.begin() + collectibleIndexToDelete);
		}
	}
	
}

void GameScreenLevel2::UpdateBlocks(float deltaTime, SDL_Event e, LevelMap* map)
{
	//note that unlike enemy and coin updates, block updates never get the block deleted.
	if (!m_blocks.empty())
	{
		for (unsigned int i = 0; i < m_blocks.size(); i++)
		{
			if (Collisions::Instance()->Box(mario->GetCollisionBox(), m_blocks[i]->GetCollisionBox()))
			{
				//only process this collision if the player was rising into the block
				if ((mario->GetVelocity().y < 0.0f) && m_blocks[i]->isAvailable() && (mario->GetCenterPosition().x > m_blocks[i]->GetPosition().x) && (mario->GetCenterPosition().x < (m_blocks[i]->GetPosition().x + m_blocks[i]->GetWidth())))
				{
					switch (m_blocks[i]->TakeHit())
					{
					case POW:
						m_score_mario += POW_SCORE;
						DoScreenshake(deltaTime);
						break;
					case DESTRUCTIBLE:
						m_score_mario += DESTROY_SCORE;
						break;
					case ITEM:
						break;
					}
					

					//set mario's velocity so he falls from the block, then correct his positioning.
					mario->SetVelocity(Vector2D(mario->GetVelocity().x, 0.0f));
					mario->SetPosition(Vector2D(mario->GetPosition().x, m_blocks[i]->GetPosition().y + m_blocks[i]->GetHeight()));
				}
			}
		}
	}
}

void GameScreenLevel2::DoScreenshake(float deltaTime)
{
	m_screenshake = true;
	m_shake_time = SHAKE_DURATION;
	m_wobble = 0.0f;

	//make all enemies on screen take damage
	for (Koopa* koopa : m_enemies)
	{
		if ((koopa->GetPosition().x > camera.x - koopa->GetWidth()) && (koopa->GetPosition().x < SCREEN_WIDTH + camera.x)) koopa->TakeDamage(deltaTime);
	}
}