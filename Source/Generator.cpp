#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "Generator.h"
#include "SimplexNoise.h"

#include "Chunk.h"
#include "Constants.h"


void Generator::RandomOffset()
{
	srand(time(NULL));
	offsetX = rand();
}
Generator::Generator(float _scale, int _minHeight, int _maxHeight)
{
	scale = _scale;
	minHeight = _minHeight;
	maxHeight = _maxHeight;
}
int Generator::getHeightAtX(int x)
{
	float noiseValue = SimplexNoise::noise((float)x / scale);
	noiseValue = (noiseValue + 1) / 2;
	int height = minHeight + noiseValue * (maxHeight - minHeight);
	return height;
}
int Generator::getCaveAt(int x, int y)
{
	float noiseValue = SimplexNoise::noise((float)x/scale, (float)y/scale);
	noiseValue = (noiseValue + 1)/2;
	if(noiseValue > 0.54 && noiseValue < 0.55)
		return 2; //Temporary resource;			
	else if(noiseValue < 0.6)
		return 1;
	else
		return 0;		
}
int Generator::getIslandAt(int x, int y)
{
	float noiseValue = SimplexNoise::noise((float)x/scale, (float)y/scale);
	noiseValue = (noiseValue + 1)/2;			
	if(noiseValue < 0.2)
		return 1;
	else
		return 0;
}
int Generator::getTileAt(int x, int y)
{
	if(y < minHeight)
	{
		return getCaveAt(x,y);
	}
	
	int height = getHeightAtX(x);
	float where = (float)y / height;
	if(where < 0.5)
			return 1;
	if(where < 0.8)
		return 2;
	if(where < 1)
		return 3;
	if(where > 1)
		{
			return getIslandAt(x, y);
			//Generate floating islands
		}
	return 0;
}
Chunk* Generator::getChunkAt(int x, int y)
{
       Tile**tiles = new Tile*[CHUNK_SIZE];
	for(int i = 0;i < CHUNK_SIZE;i++)
	{
           tiles[i] = new Tile[CHUNK_SIZE];
		for(int j = 0;j < CHUNK_SIZE;j++)
		{
			int tileID = getTileAt(x*CHUNK_SIZE+i,y*CHUNK_SIZE+ j);
			            		
				tiles[i][j].id = tileID;
		}
	}
	Chunk * c = new Chunk(tiles, x, y);
	return c;
}


