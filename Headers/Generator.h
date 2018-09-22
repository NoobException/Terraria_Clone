#ifndef GENERATOR_H
#define GENERATOR_H

#include "Chunk.h"

class Generator
{
	public:
		//Noise scale
		float scale = 50.0f;
		//Height range
		int minHeight = 2, maxHeight = 10;
		//Offset
		int offsetX;
		//Function returning new Chunk
		Chunk* getChunkAt(int x, int y);
		//Initializer
		Generator(float _scale,int _minHeight, int _maxHeight);
		void RandomOffset();
	private:
		//Get main tile at position
		int getTileAt(int, int);
		//Get height at given X
		int getHeightAtX(int);
		//Get tile under ground
		int getCaveAt(int, int);
		//Get tile above ground
		int getIslandAt(int, int);

};

#endif

