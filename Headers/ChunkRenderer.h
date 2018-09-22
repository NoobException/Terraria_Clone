#ifndef CHUNK_RENDERER_H
#define CHUNK_RENDERER_H
#include"Tile.h"
#include "Constants.h"
#include "BasicShader.h"

class ChunkRenderer{
    public:
        ChunkRenderer(Tile tiles[CHUNK_SIZE][CHUNK_SIZE]);
        ~ChunkRenderer();
        void drawWalls(int x, int y, BasicShader& shader);
        void drawFront(int x, int y, BasicShader& shader);
        void changeTile(int x, int y, Tile tiles[CHUNK_SIZE][CHUNK_SIZE]);
    private:
        static const int TILE_VERTICES_SIZE = 32, TILE_EDGES_SIZE = 6;
        static const float TILE_VERTICES_FB[TILE_VERTICES_SIZE],
TILE_VERTICES_UD[TILE_VERTICES_SIZE],TILE_VERTICES_LR[TILE_VERTICES_SIZE];
        static const unsigned int TILE_EDGES[TILE_EDGES_SIZE], FLIPPED_TILE_EDGES[TILE_EDGES_SIZE];

        unsigned int front_vao, front_vbo, front_ebo; //indices of objects in vRAM
        unsigned int walls_vao, walls_vbo, walls_ebo; //indices of objects in vRAM
        unsigned int indicesCount, walls_indicesCount;
        void generateBuffers();
        void createLRfaceInData(Tile tile, float * verticesData, unsigned int * edgesData, int offset, int x, int y, bool flipped);
        void createUDfaceInData(Tile tile, float * verticesData, unsigned int * edgesData, int offset, int x, int y, bool flipped);
        void createFrontFaceInData(Tile tile, float * verticesData, unsigned int * edgesData, int offset, int x, int y, bool flipped);


};
#endif
