#include"Player.h"

Player::Player(float x, float y):Entity(x,y, 2, 4, 0, 0, 1, 1, 1){};

void Player::update(Map& map, float deltaTime, bool left, bool right, bool jump){
   Entity::update(map,deltaTime);

   if(left&&!right){
       if(canMoveLeft(map))
           x-=15.0f*deltaTime;
       if(!canMoveLeft(map))
           x = std::round(x);
   } else if(right && !left){
       if(canMoveRight(map))
           x+=15.0f*deltaTime;
       if(!canMoveRight(map))
           x = std::round(x);
   }


   if(jump&&!canMoveDown(map)){
        verticalSpeed = -50.0f;
   }
};
