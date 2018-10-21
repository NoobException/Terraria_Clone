#include"TextureManager.h"
#include<SFML/Graphics.hpp>
#include<iostream>
#include<GL/glew.h>

TextureManager::TextureManager(){
};

TextureManager::~TextureManager(){
    for(unsigned int texture:textureHandles){
        glDeleteTextures(1, &texture);
    }

};

void TextureManager::loadTexture(const char* fileName){

    sf::Image tile_texture;
    if(!tile_texture.loadFromFile(fileName)){
        std::cout<<"Couldnt load texture"<<std::endl;
        return;
    }
    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tile_texture.getSize().x, tile_texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, tile_texture.getPixelsPtr());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    textureHandles.push_back(texture);
};

void TextureManager::useTexture(int id){

    if(id<(int)textureHandles.size())
        glBindTexture(GL_TEXTURE_2D, textureHandles.at(id));
};

