#pragma once

#include "Constants.h"
#include "Commons.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

class LevelMap
{
public:
	LevelMap(const char* first_filename, int new_tileSize);
	~LevelMap();

	void LoadLevel(const char* filename);
	TILETYPE GetTileAt(int x, int y);
	void SetTileAt(int x, int y, TILETYPE tiletype);

	unsigned int tileSize = DEFAULT_TILESIZE;
	Vector2D mapDimensions;
protected:
	fstream file;
	vector<vector<TILETYPE>> map;
	vector<TILETYPE> ReadLine();
};

