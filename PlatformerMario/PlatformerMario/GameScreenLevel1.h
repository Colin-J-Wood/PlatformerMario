#pragma once
#include <SDL.h>
#include <SDL_image.h>
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
    Mario* mario;
    Luigi* luigi;
    bool SetUpLevel();

    Texture2D* m_background_texture;
    LevelMap* m_levelmap;

    POWBlock* m_powBlock;
};

