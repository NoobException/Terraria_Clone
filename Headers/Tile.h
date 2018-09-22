#ifndef TILE_H
#define TILE_H

struct Tile{
    unsigned int id;
    bool isDirectional(){
        return id == 3;
    };
};

#endif
