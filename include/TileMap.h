#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <memory>

#include "SAFE/Vector2.h"
#include "SAFE/Vector3.h"
#include "SAFE/EntityEngine.h"
#include "SAFE/Entity.h"
#include "SAFE/Tile.h"

namespace safe {

class TileMap
{

    public:
        TileMap(EntityEngine* engine, Vector3 origin, int width, int height, int tileWidth, int tileHeight)
                : mpEngine(engine), mOrigin(origin), 
                mWidth(width), mHeight(height),
                mTileWidth(tileWidth), mTileHeight(tileHeight)
                {}


        Tile* GetTile(Vector2 pos){
            Vector2 i = World2Map(pos);
            return GetTile(i.x, i.y);
        }

        Tile* GetTile(int x, int y){
            if ( CheckBounds(x,y) ){
                return nullptr;
            }
            else{
                return mTiles[IndexOf(x,y)].get();
            }
        }

        bool CheckBounds(Vector2 pos){
            Vector2 i = World2Map(pos);
            return CheckBounds(i.x, i.y);
        }

        bool CheckBounds(int x, int y){
            return  x >= 0 && x < mWidth && y >= 0 && y < mHeight;
        }

    private:
        EntityEngine* mpEngine;
        Vector3 mOrigin;
        std::vector< std::unique_ptr< Tile > > mTiles;
        int mWidth;
        int mHeight;
        int mTileWidth;
        int mTileHeight;

        Vector2 World2Map(Vector2 pos){
            float rx = pos.x - mOrigin.x;
            float ry = pos.y - mOrigin.y;

            rx /= mTileWidth;
            ry /= mTileHeight;

            return Vector2(rx, ry);
        }

        int IndexOf (int x, int y){
            return y*mWidth + x;
        }


};

} // namespace safe

#endif // TILEMAP_H
