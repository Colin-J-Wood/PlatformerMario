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

	delete(m_text_game_over);
	delete(m_text_mario_score);
}

void GameScreenLevel2::Render()
{
	//render all tiles in front of entities so pipes can hide koopas.
	// SDL_Rect source_rect = SDL_Rect{ 0, 0, m_texture->GetWidth(), m_texture->GetHeight() };
	SDL_Rect source_rect = SDL_Rect{ 0, 0, m_background_texture->GetWidth(), m_background_texture->GetHeight() };
	SDL_Rect destination_rect = SDL_Rect{ (int)camera.x, (int)camera.y, m_background_texture->GetWidth(), m_background_texture->GetHeight() };
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

	UpdateEnemies(deltaTime, e, m_levelmap);

	//make all players stay die off screen
	if (mario->GetPosition().y > m_background_texture->GetHeight())
	{
		//keep mario off screen whilst death occurs.
		mario->SetPosition(Vector2D(mario->GetPosition().x, m_background_texture->GetHeight() + 5));
		mario->SetAlive(false);
	}
	//keep mario on screen, only vertical fallout should kill.
	if (mario->GetPosition().x < 0) mario->SetPosition(Vector2D(0, mario->GetPosition().y));

	//kill all enemies that are off boundaries, don't care if they left the side of the map so they will fall off there too.
	int indexToKill = -1;
	for (int i = 0; i < m_enemies.size(); i++)
	{
		if (m_enemies[i]->GetPosition().y > m_background_texture->GetHeight() + 5)
		{
			indexToKill = i;
		}
	}
	if (indexToKill != -1) m_enemies.erase(m_enemies.begin() + indexToKill);

	m_text_mario_score->LoadString("MARIO SCORE: " + to_string(m_score_mario), { 255, 255, 255, 255 });

	if (!mario->GetAlive())
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

	m_levelmap = new LevelMap("Maps/level2.txt", DEFAULT_TILESIZE);

	POWBlock* test_block = new POWBlock(m_renderer, m_levelmap, "Images/tile/PowBlock.png", 3);
	m_blocks.push_back(test_block);
	test_block = nullptr;

	return true;
}

void GameScreenLevel2::UpdateEnemies(float deltaTime, SDL_Event e, LevelMap* map)
{
	if (!m_enemies.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < m_enemies.size(); i++)
		{
			//enemies too far off the screen should be killed
			if ((m_enemies[i]->GetPosition().x < camera.x - 100) ||
				(m_enemies[i]->GetPosition().x > camera.x + SCREEN_WIDTH + 100)) m_enemies[i]->SetAlive(false);

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