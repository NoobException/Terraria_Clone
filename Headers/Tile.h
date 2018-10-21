#ifndef TILE_H
#define TILE_H

enum TileID {AIR,STONE, SOIL, GRASS, ORE};
struct Tile
{
    unsigned int id;
    bool isDirectional()
        {return id == GRASS;}
};

#endif
