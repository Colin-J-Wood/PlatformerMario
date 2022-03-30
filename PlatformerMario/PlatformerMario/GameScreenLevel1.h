#pragma once
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

class GameScreenLevel1 :
    public GameScreen
{
public: 
    GameScreenLevel1(SDL_Renderer* renderer);
    ~GameScreenLevel1();

    void Render() override;
    void Update(float deltaTime, SDL_Event e) override;
private:
    vector<Koopa*> m_enemies;
    Sound* m_kill_koopa;
    Mario* mario;
    Luigi* luigi;
    bool SetUpLevel();

    Texture2D* m_background_texture;
    LevelMap* m_levelmap;
    Mix_Music* m_music = nullptr;
    POWBlock* m_powBlock;

    TextRenderer* m_text;

    bool m_screenshake;
    float m_shake_time;
    float m_wobble;
    float m_background_yPos;
    float m_respawn_time;

    void DoScreenshake(float deltaTime);

    void UpdateEnemies(float deltaTime, SDL_Event e, LevelMap* map);
    void CreateKoopa(Vector2D position, FACING direction, Vector2D target_speed);
};

