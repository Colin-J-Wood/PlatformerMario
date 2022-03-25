#pragma once
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>

using namespace std;

class Sound
{
public:
	Sound(string filename);
	~Sound();

	void PlaySound(int loops);
	void ChangeSound(string filename);
private:
	Mix_Chunk* sound = nullptr;
};

