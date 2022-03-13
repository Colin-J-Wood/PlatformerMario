#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <algorithm>
#include "Koopa.h"
#include "GameScreen.h"
#include "Texture2D.h"
#include "LevelMap.h"
#include "Mario.h"
#include "Luigi.h"
#include "POWBlock.h"

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

    Mario* mario;
    Luigi* luigi;
    bool SetUpLevel();

    Texture2D* m_background_texture;
    LevelMap* m_levelmap;

    POWBlock* m_powBlock;

    bool m_screenshake;
    float m_shake_time;
    float m_wobble;
    float m_background_yPos;

    void DoScreenshake();

    void UpdateEnemies(float deltaTime, SDL_Event e, LevelMap* map);
    void CreateKoopa(Vector2D position, FACING direction, Vector2D target_speed);
};

