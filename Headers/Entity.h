#ifndef ENTITY_H
#define ENTITY_H
#include"BasicShader.h"
#include "TextureManager.h"
#include "Map.h"

class Entity{
    public:
        Entity(float x, float y, float width, float height, float textureX, float textureY, float textureWidth, float textureHeight, int textureID);
        ~Entity();
        void draw(BasicShader& shader, TextureManager& textureManager);
        static void unload();
        void update(Map& map, float deltaTime);
        float getX();
        float getY();
    protected:
        bool gravityOn = true;
        float acceleration = 30.0f;
        float x, y, width, height, textureX, textureY, textureWidth, textureHeight;
        float verticalSpeed = 0;
        int textureID;
        bool canMoveUp(Map& map);
        bool canMoveDown(Map& map);
        bool canMoveLeft(Map& map);
        bool canMoveRight(Map& map);
    private:
        static bool loaded;
        static unsigned int vao, vbo, ebo;
        static const int VERTICES_SIZE = 32, INDICES_SIZE = 6;
        static const float VERTICES[VERTICES_SIZE];
        static const unsigned int INDICES[INDICES_SIZE];
        static void load();
        bool canMoveHorizontally(Map& map, float x);
        bool canMoveVertically(Map& map, float y);
};

#endif
