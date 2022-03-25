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

    void FlipRightwayUp(float deltaTime);
    
    
public:
    Koopa(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D start_position, FACING start_facing, Vector2D movement_speed);
    ~Koopa();

    Sound* m_kill_sound;

    void TakeDamage(float deltaTime);
    void KoopaJump(float deltaTime);
    bool GetInjured() { return m_injured; };

    void Render() override;
    void Update(float deltaTime, SDL_Event e, LevelMap* map) override;
};

