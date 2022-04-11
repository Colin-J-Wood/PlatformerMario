#pragma once
#include <iostream>
#include "GameScreen.h"
#include "Texture2D.h"
#include "TextRenderer.h"
class GameScreenMenu :
    public GameScreen
{
public:
    GameScreenMenu(SDL_Renderer* renderer);
    ~GameScreenMenu();

    void Render() override;
    SCREENS Update(float deltaTime, SDL_Event e) override;

private:
    Texture2D* m_logo;
    TextRenderer* m_level_1_button;
    TextRenderer* m_level_2_button;
    TextRenderer* m_high_scores_button;

    int m_selection = 0;
};

