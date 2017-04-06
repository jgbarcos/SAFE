#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <cmath>

#include "SAFE/Vector2.h"
#include "SAFE/Vector3.h"
#include "SAFE/EntityEngine.h"

namespace safe {

class TileMap
{

public:
    TileMap(Vector3 origin, int cols, int rows, int tileWidth, int tileHeight)
            : mOrigin(origin), 
            mCols(cols), mRows(rows),
            mTileWidth(tileWidth), mTileHeight(tileHeight)
    {
        
    }

    bool CheckBounds(Vector3 pos){
        Vector2 i = World2Map(pos);
        return CheckBounds(i.x, i.y);
    }

    bool CheckBounds(int x, int y){
        return  x >= 0 && x < mCols && y >= 0 && y < mRows;
    }
    
    Vector3 SnapToMap(Vector3 pos){
        Vector3 v = Map2World(World2Map(pos));
        v.z = pos.z;
        return v;
    }

    Vector2 World2Map(Vector3 pos){
        Vector3 local = pos - mOrigin;
        
        local.x = round(local.x / (float)mTileWidth);
        local.y = round(local.y / (float)mTileHeight);

        return Vector2::Reduce(local);
    }
    
    Vector3 Map2World(Vector2 pos){        
        return Map2World(pos.x, pos.y);
    }
    
    Vector3 Map2World(int x, int y){
        return mOrigin + Vector3(x*mTileWidth, y*mTileHeight,0);
    }
    
    int GetCols(){ return mCols; }
    int GetRows(){ return mRows; }
    
    bool IsEmpty(int x, int y){
        return mEntitiesPosition[x + y*mCols].empty();
    }
    
    bool SetUnit(int x, int y, EntityEngine::EntityID id){
        if(CheckBounds(x, y)){
            mEntitiesPosition[x + y*mCols].push_back(id);
            return true;
        }
        return false;
    }
    
    std::unordered_map< int, std::vector<EntityEngine::EntityID> > mEntitiesPosition;
    

private:
    Vector3 mOrigin;
    int mCols;
    int mRows;
    int mTileWidth;
    int mTileHeight;
    
};

} // namespace safe

#endif // TILEMAP_H
