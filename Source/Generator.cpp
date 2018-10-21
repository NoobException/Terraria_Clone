#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Generator.h"
#include "SimplexNoise.h"

#include "Chunk.h"
#include "Constants.h"

#include "Logger.h"


void Generator::RandomOffset()
{
	srand(time(NULL));
	offsetX = rand() % 1000;
	offsetY = rand() % 1000;
}

float Generator::getNoiseAt(int x, int y)
{
	return SimplexNoise::noise((float)x/scale + offsetX, (float)y/scale + offsetX);
}

float Generator::getNoiseAt(int x)
{
	return SimplexNoise::noise((float)x / scale + offsetX);
}

//Constructor
Generator::Generator(float _scale, int _minHeight, int _maxHeight)
{
	scale = _scale;
	minHeight = _minHeight;
	maxHeight = _maxHeight;
	RandomOffset();
}

int Generator::getHeightAtX(int x)
{
	float noiseValue = getNoiseAt(x);
	noiseValue = (noiseValue + 1) / 2;
	int height = minHeight + noiseValue * (maxHeight - minHeight);
	return height;
}

int Generator::getCaveAt(int x, int y)
{

	float noiseValue = getNoiseAt(x, y);
	noiseValue = (noiseValue + 1)/2; 
	if(noiseValue > 0.54 && noiseValue < 0.55)
		return ORE; //Temporary resource;			
	else if(noiseValue < 0.6)
		return STONE;
	else
		return AIR;		
}
int Generator::getIslandAt(int x, int y)
{
	float noiseValue = getNoiseAt(x, y);
	noiseValue = (noiseValue + 1)/2;			
	if(noiseValue < 0.2)
		return 1;
	else
		return 0;
}


int Generator::getTileAt(int x, int y)
{
	int height = getHeightAtX(x);
	float where = (float)y / height;
	if(where < 0.2)
			return STONE;
	if(where < 1)
		return SOIL;
	if(where == 1)
		return GRASS;
		
	return 0;
}


void Generator::digCaves(int chunkX, int chunkY, Tile** tiles)
{
	for(int x = 0; x < CHUNK_SIZE; x++)
	{
		int wormX = chunkX * CHUNK_SIZE + x;
		int wormY =(getNoiseAt(wormX) - 0.5) * 20;
		wormY = wormY - chunkY * CHUNK_SIZE;
		for(int dx = -3; dx < 4; dx++)
		{
			for(int dy = -3; dy < 4; dy++)
			{
				int headRadius = (getNoiseAt(wormX) + 1) * 4;
				if(dx * dx + dy*dy > headRadius * headRadius)
					continue;
				if(x + dx >= 0 && x + dx < CHUNK_SIZE && wormY + dy >= 0 && wormY + dy < CHUNK_SIZE)
				{
					tiles[x + dx][wormY + dy].id = AIR;
				}
			}
		}

	}
}
Chunk* Generator::getChunkAt(int chunkX, int chunkY)
{
	Tile** tiles = new Tile*[CHUNK_SIZE];
	for(int x = 0; x < CHUNK_SIZE; x++)
	{
		tiles[x] = new Tile[CHUNK_SIZE];
		for(int y = 0; y < CHUNK_SIZE; y++)
		{
			int tileID = getTileAt(chunkX * CHUNK_SIZE + x, chunkY * CHUNK_SIZE + y);
			tiles[x][y].id = tileID;
		}
	}
	digCaves(chunkX, chunkY, tiles);

	Chunk* generated = new Chunk(tiles, chunkX, chunkY);
	return generated;
}


