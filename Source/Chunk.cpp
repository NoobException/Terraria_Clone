#include "Chunk.h"

Chunk::Chunk(Tile ** tiles, int x, int y){
    this->x = x;
    this->y = y;

    for(int i = 0; i<CHUNK_SIZE; i++){
        for(int j = 0; j<CHUNK_SIZE; j++){
            map[i][j] = tiles[i][j];
        }
    }

    renderer = new ChunkRenderer(map);
};
Chunk::~Chunk(){
    delete renderer;
};
void Chunk::setTile(int x, int y, Tile tile){
    map[x][y] = tile;
    if(renderer!=nullptr){
        renderer->changeTile(x, y, map);
    }
};
void Chunk::drawWalls(BasicShader&shader){
    if(renderer != nullptr){
        renderer->drawWalls(x, y, shader);
    }
}
void Chunk::drawFront(BasicShader&shader){
    if(renderer != nullptr){
        renderer->drawFront(x, y, shader);
    }
}
