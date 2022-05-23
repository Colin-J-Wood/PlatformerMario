#pragma once
#include "GameScreen.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <iostream>
#include "Koopa.h"
#include "GameScreen.h"
#include "Texture2D.h"
#include "LevelMap.h"
#include "Mario.h"
#include "Luigi.h"
#include "POWBlock.h"
#include "TextRenderer.h"
#include "Character.h"
#include "Collisions.h"
#include "Collectible.h"

class GameScreenLevel2 :
    public GameScreen
{
public:
    GameScreenLevel2(SDL_Renderer* renderer);
    ~GameScreenLevel2();

    void Render() override;
    SCREENS Update(float deltaTime, SDL_Event e) override;

private:
    Rect2D camera = Rect2D(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    vector<Koopa*> m_enemies;
    vector<POWBlock*> m_blocks;
    vector<Collectible*> m_collectibles;
    Sound* m_collect_coin;
    Sound* m_kill_koopa;
    Mario* mario;

    Texture2D* m_background_texture;
    LevelMap* m_levelmap;
    Mix_Music* m_music;

    TextRenderer* m_text_mario_score;
    TextRenderer* m_text_game_over;

    bool m_screenshake;
    float m_shake_time;
    float m_wobble;
    float m_background_yPos;

    float m_respawn_time;
    float m_coin_respawn_time;

    int m_score_mario;
    float m_game_over_time;

    bool SetUpLevel();

    void SetupCollectibles();
    void SetupBlocks();

    void UpdateEnemies(float deltaTime, SDL_Event e, LevelMap* map);
    void UpdateCollectibles(float deltaTime, SDL_Event e, LevelMap* map);
    void UpdateBlocks(float deltaTime, SDL_Event e, LevelMap* map);

    void DoScreenshake(float deltaTime);
    void CreateKoopa(Vector2D position, FACING direction, Vector2D target_speed);
};

