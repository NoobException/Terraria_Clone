#include"Entity.h"
#include<GL/glew.h>
#include<iostream>

bool Entity::loaded = false;
const float Entity::VERTICES[] = {
    0.0f, 0.0f, 0.5f, 0.01f, 0.99f, 0.0f, 0.0f, -1.0f,
    0.0f, 1.0f, 0.5f, 0.01f, 0.01f, 0.0f, 0.0f, -1.0f,
    1.0f, 1.0f, 0.5f, 0.99f, 0.01f, 0.0f, 0.0f, -1.0f,
    1.0f, 0.0f, 0.5f, 0.99f, 0.99f, 0.0f, 0.0f, -1.0f
};
const unsigned int Entity::INDICES[] = {
    0, 2, 1,
    0, 3, 2
};
unsigned int Entity::vao = 0, Entity::vbo = 0, Entity::ebo = 0;

Entity::Entity(float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight, int textureID){
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->textureX = textureX;
    this->textureY = textureY;
    this->textureWidth = textureWidth;
    this->textureHeight = textureHeight;
    this->textureID = textureID;

    if(!loaded){
        load();
    }
};
float Entity::getX(){
    return x;
}
float Entity::getY(){
    return y;
}
void Entity::update(Map& map, float deltaTime){
    if(gravityOn){
        if(verticalSpeed==0||(verticalSpeed>0&&canMoveDown(map))||(verticalSpeed<0&&canMoveUp(map))){
            y-=(verticalSpeed + 0.5*acceleration*deltaTime)*deltaTime;
            verticalSpeed+=3*acceleration*deltaTime;
        } else{
            y = std::round(y);
            verticalSpeed = 0;
        }
    }
};
bool Entity::canMoveVertically(Map& map, float y){
    float point = x+0.01f;
    bool ans = map.isAir(point, y);
    while(point<x+width-0.01f&&ans){
        ans &= map.isAir(point, y);
        point += 1;
    }
    ans &= map.isAir(x+width-0.01f, y);
    return ans;
}
bool Entity::canMoveHorizontally(Map& map, float x){
    float point = y+0.01f;
    bool ans = map.isAir(x, point);
    while(point<y+height-0.01f&&ans){
        ans &= map.isAir(x, point);
        point+=1;
    }
    ans &= map.isAir(x, y+height-0.01f);
    return ans;
}
bool Entity::canMoveLeft(Map& map){
    return canMoveHorizontally(map, x);
}
bool Entity::canMoveRight(Map& map){
    return canMoveHorizontally(map, x+width);
}
bool Entity::canMoveDown(Map& map){
    return canMoveVertically(map, y);
}
bool Entity::canMoveUp(Map& map){
    return canMoveVertically(map, y+height);
}
void Entity::draw(BasicShader& shader, TextureManager& textureManager){
    if(loaded){
        textureManager.useTexture(textureID);
        shader.use();
        shader.setScale(width, height);
        shader.setTranslation(x, y);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, INDICES_SIZE, GL_UNSIGNED_INT, (GLvoid*) 0);
        glBindVertexArray(0);
    }
};

void Entity::unload(){
    if(loaded){
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        loaded = false;
    }
};
void Entity::load(){
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, VERTICES_SIZE*sizeof(GLfloat), VERTICES, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*) 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*) (3*sizeof(GLfloat)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*) (5*sizeof(GLfloat)));

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES_SIZE*sizeof(GLuint), INDICES, GL_STATIC_DRAW);

    glBindVertexArray(0);
    loaded = true;
};
Entity::~Entity(){

};

