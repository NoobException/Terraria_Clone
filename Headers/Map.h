#ifndef MAP_H
#define MAP_H
#include<vector>
#include"Chunk.h"

class Map{
    public:
        Map();
        ~Map();
        void draw(BasicShader& shader);
    private:
        std::vector<Chunk *> activeChunks;
        
};
#endif
