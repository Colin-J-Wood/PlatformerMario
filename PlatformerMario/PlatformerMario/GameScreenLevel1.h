#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "GameScreen.h"
#include "Texture2D.h"

class GameScreenLevel1 :
    public GameScreen
{
public: 
    GameScreenLevel1(SDL_Renderer* renderer);
    ~GameScreenLevel1();

    void Render() override;
    void Update(float deltaTime, SDL_Event e) override;
private:
    bool SetUpLevel();

    Texture2D* m_background_texture;
};

