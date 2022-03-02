#include "LevelMap.h"

//use of this version of the constructor is recommended.  it will load the map on start.
LevelMap::LevelMap(const char* first_filename, int new_tileSize = DEFAULT_TILESIZE)
{
	//loading the level also sets the map dimensions.
	LoadLevel(first_filename);
	tileSize = new_tileSize;
}

LevelMap::~LevelMap()
{
	//clear the vector so we are assured it is destroyed.
	map.clear();
}

//loads the level map from a file
void LevelMap::LoadLevel(const char* filename)
{
	//file is a protected variable of this class
	//for the purpose of referencing the same fstream between multiple methods.
	file.open(filename, ios::in);

	if (!file.is_open())
	{
		//exit if the file never opened.
		cout << "Problem opening file for reading" << endl;
		cout << filename << " failed to open." << endl;
		return;
	}

	//read all the lines, filling the map.
	while (!file.eof())
	{
		//map is a vector of lines in the file.
		//so we load all lines into the vector to get our map.
		map.push_back(ReadLine());
	}
	
	file.close();
}

vector<TILETYPE> LevelMap::ReadLine()
{
	//this vector is a single line in the file.
	vector<TILETYPE> tileLine;

	//first get the line as a string.
	string lineOfText;
	getline(file, lineOfText);

	//read for integers between commas
	stringstream ss(lineOfText);
	while (ss.good())
	{
		string substr;
		getline(ss, substr, ',');
		//validate it is a proper tile number when converting to the enum
		tileLine.push_back(TILETYPE (stoi(substr)));
	}

	return tileLine;
}

//x,y are tile indexes, not coordinate positions!
TILETYPE LevelMap::GetTileAt(unsigned int x, unsigned int y)
{
	//boundary safe lookup, return AIR if it's not a valid index of the vector.
	TILETYPE tiletype = AIR;
	vector<TILETYPE> row;

	if (y > map.size())
	{
		//uncomment to debug.
		cout << "Invalid map height index, counting as AIR." << endl;
		return AIR;
	}
	else
	{
		row = map[y];
	}

	if (x > row.size())
	{
		//uncomment to debug.
		cout << "Invalid map width index, counting as AIR." << endl;
		return AIR;
	}
	else
	{
		return row[x];
	}
}

//x,y are tile indexes, not coordinate positions!
void LevelMap::SetTileAt(unsigned int x, unsigned int y, TILETYPE tiletype)
{
	vector<TILETYPE> row;

	//boundary safe vector setting.
	//exit this function early if the index passed is invalid.
	if (y > map.size())
	{
		cout << "Invalid map height index, rejecting tile setter." << endl;
		return;
	}
	else
	{
		row = map[y];
	}

	if (x > map.size())
	{
		cout << "Invalid map width index, rejecting tile setter." << endl;
		return;
	}
	else
	{
		row[x] = tiletype;
	}
}

