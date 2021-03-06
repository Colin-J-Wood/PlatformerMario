#pragma once

//screen dimensions
#define SCREEN_WIDTH 511
#define SCREEN_HEIGHT 407
#define TARGET_MILLISEC 4

#define PLAYER_SPEED Vector2D(85.0f, 225.0f)
#define PLAYER_JUMP_TIME 0.5f
#define PLAYER_COLLISION_RADIUS 15.0f
#define PLAYER_DEATH_TIME 5.0f

#define MAP_WIDTH
#define DEFAULT_TILESIZE 16

#define GRAVITY 1.5f

#define SHAKE_DURATION 0.5f

#define POW_SCORE 500
#define DESTROY_SCORE 200
#define KOOPA_SCORE 800
#define COLLECTIBLE_SCORE 10000
#define DESTROY_DAMAGE_SCORE 2000

#define KOOPA_INJURED_TIME 10.0f
#define KOOPA_SPEED Vector2D(75.0f, 75.0f)
#define KOOPA_SPEED_2 Vector2D(37.5f, 75.0f)
#define KOOPA_RESPAWN_TIME 2.0f

#define COLLECTIBLE_RADIUS 16.0f
#define DESTRUCTIBLE_TOLERANCE 4 //defines an amount of pixels the offset can be for a tile hit to not count

#define DESTRUCTIBLE_RESPAWN_TIME 10.0f
#define COLLECTIBLE_RESPAWN_TIME 30.0f