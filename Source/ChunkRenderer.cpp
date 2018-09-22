#include"ChunkRenderer.h"
#include"Constants.h"
#include<GL/glew.h>
#include<iostream>

const float ChunkRenderer::TILE_VERTICES_FB[] = {
    0.0f, 0.0f, 0.0f, 0.05f, 0.95f, 0.0f, 0.0f, -1.0f,
    0.0f, 1.0f, 0.0f, 0.05f, 0.05f, 0.0f, 0.0f, -1.0f,
    1.0f, 1.0f, 0.0f, 0.95f, 0.05f, 0.0f, 0.0f, -1.0f,
    1.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.0f, 0.0f, -1.0f
};

const float ChunkRenderer::TILE_VERTICES_LR[] = {
    0.0f, 0.0f, 0.0f, 0.05f, 0.95f, 1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 1.0f, 0.95f, 0.05f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.95f, 0.95f, 1.0f, 0.0f, 0.0f
};
const float ChunkRenderer::TILE_VERTICES_UD[] = {
    0.0f, 0.0f, 0.0f, 0.05f, 0.95f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.05f, 0.05f, 0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 1.0f, 0.95f, 0.05f, 0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.0f, 1.0f, 0.0f
};
const unsigned int ChunkRenderer::TILE_EDGES[] = {
    0, 2, 1,
    0, 3, 2
};
const unsigned int ChunkRenderer::FLIPPED_TILE_EDGES[] = {
    0, 1, 2,
    0, 2, 3
};

ChunkRenderer::ChunkRenderer(Tile tiles[CHUNK_SIZE][CHUNK_SIZE]){
    float * verticesData = new float[CHUNK_SIZE*CHUNK_SIZE*TILE_VERTICES_SIZE];
    unsigned int * edgesData = new unsigned int[CHUNK_SIZE*CHUNK_SIZE*TILE_EDGES_SIZE];
    float * walls_verticesData = new float[2*(CHUNK_SIZE+1)*CHUNK_SIZE*TILE_VERTICES_SIZE];
    unsigned int * walls_edgesData = new unsigned int[2*(CHUNK_SIZE+1)*CHUNK_SIZE*TILE_VERTICES_SIZE];
    int half = CHUNK_SIZE*(CHUNK_SIZE+1);
    for(int i = 0; i<CHUNK_SIZE*CHUNK_SIZE*TILE_VERTICES_SIZE; i++)
        verticesData[i] = 0;
    for(int i = 0; i<CHUNK_SIZE*CHUNK_SIZE*TILE_EDGES_SIZE; i++)
        edgesData[i] = 0;
    for(int i = 0; i<2*half*TILE_VERTICES_SIZE; i++)
        walls_verticesData[i] = 0;
    for(int i = 0; i<2*half*TILE_EDGES_SIZE; i++)
        walls_edgesData[i] = 0;
    for(int i = 0; i<CHUNK_SIZE; i++){
        for(int j = 0; j<CHUNK_SIZE; j++){
            if(tiles[i][j].id){
                createFrontFaceInData(tiles[i][j], verticesData, edgesData, i+j*CHUNK_SIZE, i, j, false);
                if(i==0||!tiles[i-1][j].id){
                    createLRfaceInData(tiles[i][j], walls_verticesData, walls_edgesData, (i+j*(CHUNK_SIZE+1)), i, j, true);
                }
                if(i==CHUNK_SIZE-1||!tiles[i+1][j].id){
                    createLRfaceInData(tiles[i][j], walls_verticesData, walls_edgesData, (i+1+j*(CHUNK_SIZE+1)), i+1, j, false);
                }
                if(j==0||!tiles[i][j-1].id){
                    createUDfaceInData(tiles[i][j], walls_verticesData, walls_edgesData, i*(CHUNK_SIZE+1)+j+half, i, j, true);
                }
                if(j==CHUNK_SIZE-1||!tiles[i][j+1].id){
                    createUDfaceInData(tiles[i][j], walls_verticesData, walls_edgesData, 1+i*(CHUNK_SIZE+1)+j+half, i, j+1, false);
                }
            }
        }
    }
    indicesCount = CHUNK_SIZE*CHUNK_SIZE*TILE_EDGES_SIZE;
    walls_indicesCount = 2*CHUNK_SIZE*(CHUNK_SIZE+1)*TILE_EDGES_SIZE;


    glGenVertexArrays(1, &front_vao);
    glBindVertexArray(front_vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &front_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, front_vbo);
    glBufferData(GL_ARRAY_BUFFER, CHUNK_SIZE*CHUNK_SIZE*TILE_VERTICES_SIZE*sizeof(GLfloat), verticesData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*) 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*) (3*sizeof(GLfloat)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*) (5*sizeof(GLfloat)));

    glGenBuffers(1, &front_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, front_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount*sizeof(GLuint), edgesData, GL_STATIC_DRAW);

    glBindVertexArray(0);

    glGenVertexArrays(1, &walls_vao);
    glBindVertexArray(walls_vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &walls_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, walls_vbo);
    glBufferData(GL_ARRAY_BUFFER, 2*((CHUNK_SIZE+1)*CHUNK_SIZE)*TILE_VERTICES_SIZE*sizeof(GLfloat), walls_verticesData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*) 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*) (3*sizeof(GLfloat)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*) (5*sizeof(GLfloat)));

    glGenBuffers(1, &walls_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, walls_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, walls_indicesCount*sizeof(GLuint), walls_edgesData, GL_STATIC_DRAW);

    glBindVertexArray(0);
    delete[] verticesData;
    delete[] edgesData;
    delete[] walls_verticesData;
    delete[] walls_edgesData;
};
void ChunkRenderer::changeTile(int x, int y, Tile tiles[CHUNK_SIZE][CHUNK_SIZE]){
    float * vertexData = new float[TILE_VERTICES_SIZE];
    unsigned int * edgeData = new unsigned int[TILE_EDGES_SIZE];
    int half = CHUNK_SIZE*(CHUNK_SIZE+1);
    glBindVertexArray(front_vao);
    glBindBuffer(GL_ARRAY_BUFFER, front_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, front_ebo);
    for(int i = 0; i<TILE_VERTICES_SIZE; i++){

        vertexData[i] = 0.0f;
    }
    for(int i = 0; i<TILE_EDGES_SIZE; i++){
        edgeData[i] = 0;
    }
    if(tiles[x][y].id)
        createFrontFaceInData(tiles[x][y], vertexData, edgeData, 0, x, y, false);

    glBufferSubData(GL_ARRAY_BUFFER, (x+y*CHUNK_SIZE)*TILE_VERTICES_SIZE*sizeof(GLfloat), TILE_VERTICES_SIZE*sizeof(GLfloat), vertexData);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (x+y*CHUNK_SIZE)*TILE_EDGES_SIZE*sizeof(GLuint), TILE_EDGES_SIZE*sizeof(GLuint), edgeData);

    glBindVertexArray(walls_vao);
    glBindBuffer(GL_ARRAY_BUFFER, walls_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, walls_ebo);
    if(tiles[x][y].id &&(x==0||!tiles[x-1][y].id)){
        createLRfaceInData(tiles[x][y], vertexData, edgeData, 0, x, y, true);
    }else if(!tiles[x][y].id){
        if(x!=0&&tiles[x-1][y].id)
            createLRfaceInData(tiles[x-1][y], vertexData, edgeData, 0, x, y, false);
        else{
            for(int i = 0; i<TILE_VERTICES_SIZE; i++)
                vertexData[i] = 0.0f;
            for(int i = 0; i<TILE_EDGES_SIZE; i++)
                edgeData[i] = 0;
        }
    }
    glBufferSubData(GL_ARRAY_BUFFER, (x+y*(CHUNK_SIZE+1))*TILE_VERTICES_SIZE*sizeof(GLfloat), TILE_VERTICES_SIZE*sizeof(GLfloat), vertexData);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (x+y*(CHUNK_SIZE+1))*TILE_EDGES_SIZE*sizeof(GLuint), TILE_EDGES_SIZE*sizeof(GLuint), edgeData);
    if(tiles[x][y].id &&(x==CHUNK_SIZE-1||!tiles[x+1][y].id)){
        if(tiles[x][y].id)
            createLRfaceInData(tiles[x][y], vertexData, edgeData, 0, x+1, y, false);
    }else if(!tiles[x][y].id){
        if(x!=CHUNK_SIZE-1&&tiles[x+1][y].id)
            createLRfaceInData(tiles[x+1][y], vertexData, edgeData, 0, x+1, y, true);
        else{
            for(int i = 0; i<TILE_VERTICES_SIZE; i++)
                vertexData[i] = 0.0f;
            for(int i = 0; i<TILE_EDGES_SIZE; i++)
                edgeData[i] = 0;
        }
    }
    glBufferSubData(GL_ARRAY_BUFFER, (x+1+y*(CHUNK_SIZE+1))*TILE_VERTICES_SIZE*sizeof(GLfloat), TILE_VERTICES_SIZE*sizeof(GLfloat), vertexData);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (x+1+y*(CHUNK_SIZE+1))*TILE_EDGES_SIZE*sizeof(GLuint), TILE_EDGES_SIZE*sizeof(GLuint), edgeData);
    if(tiles[x][y].id &&(y==0||!tiles[x][y-1].id)){
        createUDfaceInData(tiles[x][y], vertexData, edgeData, 0, x, y, true);
    }else if(!tiles[x][y].id){
        if(y!=0&&tiles[x][y-1].id)
            createUDfaceInData(tiles[x][y-1], vertexData, edgeData, 0, x, y, false);
        else{
            for(int i = 0; i<TILE_VERTICES_SIZE; i++)
                vertexData[i] = 0.0f;
            for(int i = 0; i<TILE_EDGES_SIZE; i++)
                edgeData[i] = 0;
        }
    }
    glBufferSubData(GL_ARRAY_BUFFER, (x*(CHUNK_SIZE+1)+y+half)*TILE_VERTICES_SIZE*sizeof(GLfloat), TILE_VERTICES_SIZE*sizeof(GLfloat), vertexData);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (x*(CHUNK_SIZE+1)+y+half)*TILE_EDGES_SIZE*sizeof(GLuint), TILE_EDGES_SIZE*sizeof(GLuint), edgeData);
    if(tiles[x][y].id &&(y==CHUNK_SIZE-1||!tiles[x][y+1].id)){
        createUDfaceInData(tiles[x][y], vertexData, edgeData, 0, x, y+1, false);
    }else if(!tiles[x][y].id){
        if(y!=CHUNK_SIZE-1&&tiles[x][y+1].id)
            createUDfaceInData(tiles[x][y+1], vertexData, edgeData, 0, x, y+1, true);
        else{
            for(int i = 0; i<TILE_VERTICES_SIZE; i++)
                vertexData[i] = 0.0f;
            for(int i = 0; i<TILE_EDGES_SIZE; i++)
                edgeData[i] = 0;
        }
    }
    glBufferSubData(GL_ARRAY_BUFFER, (x*(CHUNK_SIZE+1)+y+1+half)*TILE_VERTICES_SIZE*sizeof(GLfloat), TILE_VERTICES_SIZE*sizeof(GLfloat), vertexData);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (x*(CHUNK_SIZE+1)+y+1+half)*TILE_EDGES_SIZE*sizeof(GLuint), TILE_EDGES_SIZE*sizeof(GLuint), edgeData);
};

void ChunkRenderer::createLRfaceInData(Tile tile, float * verticesData, unsigned int * edgesData, int offset, int x, int y, bool flipped){

    for(int k = 0; k<4; k++){
        verticesData[offset*TILE_VERTICES_SIZE + k*8] = TILE_VERTICES_LR[k*8]+x;
        verticesData[offset*TILE_VERTICES_SIZE + k*8+1] = TILE_VERTICES_LR[k*8+1]+y;
        verticesData[offset*TILE_VERTICES_SIZE + k*8+2] = TILE_VERTICES_LR[k*8+2];
        verticesData[offset*TILE_VERTICES_SIZE + k*8+3] = TILE_VERTICES_LR[k*8+3]+5*(tile.id-1)/16;
        verticesData[offset*TILE_VERTICES_SIZE + k*8+4] = TILE_VERTICES_LR[k*8+4]+(tile.id-1)%16;
        verticesData[offset*TILE_VERTICES_SIZE + k*8+5] = (flipped?-1:1)*TILE_VERTICES_LR[k*8+5];
        verticesData[offset*TILE_VERTICES_SIZE + k*8+6] = TILE_VERTICES_LR[k*8+6];
        verticesData[offset*TILE_VERTICES_SIZE + k*8+7] = TILE_VERTICES_LR[k*8+7];

    };
    for(int k=0; k<TILE_EDGES_SIZE; k++){
        if(flipped)
            edgesData[offset*TILE_EDGES_SIZE+k] = FLIPPED_TILE_EDGES[k]+(x+y*(CHUNK_SIZE+1))*4;
        else
            edgesData[offset*TILE_EDGES_SIZE+k] = TILE_EDGES[k]+(x+y*(CHUNK_SIZE+1))*4;
    }
};
void ChunkRenderer::createUDfaceInData(Tile tile, float * verticesData, unsigned int * edgesData, int offset, int x, int y, bool flipped){

    int half = CHUNK_SIZE*(CHUNK_SIZE+1);
    for(int k = 0; k<4; k++){
        verticesData[(offset)*TILE_VERTICES_SIZE + k*8] = TILE_VERTICES_UD[k*8]+x;
        verticesData[(offset)*TILE_VERTICES_SIZE + k*8+1] = TILE_VERTICES_UD[k*8+1]+y;
        verticesData[(offset)*TILE_VERTICES_SIZE + k*8+2] = TILE_VERTICES_UD[k*8+2];
        verticesData[(offset)*TILE_VERTICES_SIZE + k*8+3] = TILE_VERTICES_UD[k*8+3]+5*(tile.id-1)/16+3+flipped;
        verticesData[(offset)*TILE_VERTICES_SIZE + k*8+4] = TILE_VERTICES_UD[k*8+4]+(tile.id-1)%16;
        verticesData[(offset)*TILE_VERTICES_SIZE + k*8+5] = TILE_VERTICES_UD[k*8+5];
        verticesData[(offset)*TILE_VERTICES_SIZE + k*8+6] = (flipped?-1:1)*TILE_VERTICES_UD[k*8+6];
        verticesData[(offset)*TILE_VERTICES_SIZE + k*8+7] = TILE_VERTICES_UD[k*8+7];

    };
    for(int k=0; k<TILE_EDGES_SIZE; k++){
        if(flipped)
            edgesData[(offset)*TILE_EDGES_SIZE+k] = FLIPPED_TILE_EDGES[k]+(half+x*(CHUNK_SIZE+1)+y)*4;
        else
            edgesData[(offset)*TILE_EDGES_SIZE+k] = TILE_EDGES[k]+(half+x*(CHUNK_SIZE+1)+y)*4;
    }
};
void ChunkRenderer::createFrontFaceInData(Tile tile, float * verticesData, unsigned int * edgesData, int offset, int x, int y, bool flipped){
    for(int k = 0; k<4; k++){
        verticesData[offset*TILE_VERTICES_SIZE + k*8] = TILE_VERTICES_FB[k*8]+x;
        verticesData[offset*TILE_VERTICES_SIZE + k*8+1] = TILE_VERTICES_FB[k*8+1]+y;
        verticesData[offset*TILE_VERTICES_SIZE + k*8+2] = TILE_VERTICES_FB[k*8+2]+flipped;
        verticesData[offset*TILE_VERTICES_SIZE + k*8+3] = TILE_VERTICES_FB[k*8+3]+5*(tile.id-1)/16+1+flipped;
        verticesData[offset*TILE_VERTICES_SIZE + k*8+4] = TILE_VERTICES_FB[k*8+4]+(tile.id-1)%16;
        verticesData[offset*TILE_VERTICES_SIZE + k*8+5] = TILE_VERTICES_FB[k*8+5];
        verticesData[offset*TILE_VERTICES_SIZE + k*8+6] = TILE_VERTICES_FB[k*8+6];
        verticesData[offset*TILE_VERTICES_SIZE + k*8+7] = TILE_VERTICES_FB[k*8+7];

    };
    for(int k=0; k<TILE_EDGES_SIZE; k++){
            edgesData[offset*TILE_EDGES_SIZE+k] = TILE_EDGES[k]+(x+y*CHUNK_SIZE)*4;
    }
};
ChunkRenderer::~ChunkRenderer(){
    glDeleteVertexArrays(1, &walls_vao);
    glDeleteBuffers(1, &walls_vbo);
    glDeleteBuffers(1, &walls_ebo);
    glDeleteVertexArrays(1, &front_vao);
    glDeleteBuffers(1, &front_vbo);
    glDeleteBuffers(1, &front_ebo);
};
void ChunkRenderer::drawWalls(int x, int y, BasicShader& shader){
    shader.use();
    shader.setTranslation(x*CHUNK_SIZE, y*CHUNK_SIZE);
    glBindVertexArray(walls_vao);
    glDrawElements(GL_TRIANGLES, walls_indicesCount, GL_UNSIGNED_INT, (GLvoid*) 0);
    glBindVertexArray(0);
};
void ChunkRenderer::drawFront(int x, int y, BasicShader& shader){
    shader.use();
    shader.setTranslation(x*CHUNK_SIZE, y*CHUNK_SIZE);
    glBindVertexArray(front_vao);
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, (GLvoid*) 0);
    glBindVertexArray(0);
};
