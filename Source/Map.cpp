#include "Map.h"
#include"Generator.h"

Map::Map(){
    Generator generator(50,10,30);
    for(int chunkX = -5; chunkX < 5; chunkX++)
    for(int chunkY = -2; chunkY < 5; chunkY++)
        activeChunks.push_back(generator.getChunkAt(chunkX, chunkY));

};
void Map::draw(BasicShader& shader){
    for(Chunk* c:activeChunks){
        c->drawWalls(shader);
    }
    for(Chunk* c:activeChunks){
        c->drawFront(shader);
    }
};

Map::~Map(){
    for(Chunk* c:activeChunks){
        delete c;
    }
};

