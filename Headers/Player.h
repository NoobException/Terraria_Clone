#ifndef PLAYER_H
#define PLAYER_H
#include"Entity.h"

class Player: public Entity{
    public:
        Player(float x, float y);
        void update(Map& map, float deltaTime, bool left, bool right, bool jump);
};

#endif

