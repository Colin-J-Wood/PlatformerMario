#include "Sound.h"

using namespace std;

Sound::Sound(string filename)
{
	sound = Mix_LoadWAV(filename.c_str());

	//if the sound failed to load...
	if (sound == nullptr)
	{
		cout << "Error!  Sound was not loaded: " << filename << endl;
	}
}

Sound::~Sound()
{
	//only destroy if a sound was properly loaded
	if (sound != nullptr) Mix_FreeChunk(sound);
}

void Sound::ChangeSound(string filename)
{
	//if there was a sound properly loaded before...
	if (sound != nullptr)
	{
		//free it, and load the new one.
		Mix_FreeChunk(sound);
		sound = Mix_LoadWAV(filename.c_str());
		
		if (sound == nullptr)
		{
			cout << "Error!  Sound was not loaded: " << filename << endl;
		}
	}
}

void Sound::PlaySound(int loops)
{
	if (sound != nullptr) Mix_PlayChannel(-1, sound, loops);
}