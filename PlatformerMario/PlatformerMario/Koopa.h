#pragma once
#include "Character.h"
class Koopa :
    public Character
{
private:
    float m_single_sprite_w;
    float m_single_sprite_h;

    bool m_injured;
    float m_injured_time;

    void FlipRightwayUp();
    
public:
    Koopa();
    ~Koopa();

    void TakeDamage();
    void Jump();

    void Render() override;
    void Update(float deltaTime, SDL_Event e, LevelMap* map) override;
};

