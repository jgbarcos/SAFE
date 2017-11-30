#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <queue>
#include <cmath>

#include "SAFE/Vector2.h"
#include "SAFE/Vector3.h"

#include "SAFE/EntityEngine.h"

class TileNode {
public:

    TileNode(int x, int y, int cost)
    : mX(x), mY(y), mCost(cost) { }

    bool operator<(const TileNode& other) const {
        return mCost < other.mCost;
    }

    int mX;
    int mY;
    int mCost;
};

class TileMap {
public:

    TileMap(safe::Vector3 origin, int cols, int rows, int tileWidth, int tileHeight)
    : mOrigin(origin),
    mCols(cols), mRows(rows),
    mTileWidth(tileWidth), mTileHeight(tileHeight) { }

    bool CheckBounds(safe::Vector3 pos) {
        safe::Vector2 i = World2Map(pos);
        return CheckBounds(i.x, i.y);
    }

    bool CheckBounds(int x, int y) {
        return x >= 0 && x < mCols && y >= 0 && y < mRows;
    }

    safe::Vector3 SnapToMap(safe::Vector3 pos) {
        safe::Vector3 v = Map2World(World2Map(pos));
        v.z = pos.z;
        return v;
    }

    safe::Vector2 World2Map(safe::Vector3 pos) {
        safe::Vector3 local = pos - mOrigin;

        local.x = round(local.x / (float) mTileWidth);
        local.y = round(local.y / (float) mTileHeight);

        return safe::Vector2::Reduce(local);
    }

    safe::Vector3 Map2World(safe::Vector2 pos) {
        return Map2World(pos.x, pos.y);
    }

    safe::Vector3 Map2World(safe::Vector3 pos) {
        safe::Vector3 res = Map2World(safe::Vector2::Reduce(pos));
        res.z = pos.z;
        return res;
    }

    safe::Vector3 Map2World(int x, int y) {
        return mOrigin + safe::Vector3(x*mTileWidth, y*mTileHeight, 0);
    }

    safe::Vector3 Map2World(int x, int y, double z) {
        safe::Vector3 pos = Map2World(x, y);
        pos.z = z;
        return pos;
    }

    int GetCols() {
        return mCols;
    }

    int GetRows() {
        return mRows;
    }

    bool IsEmpty(int x, int y) {
        if (mEntitiesPosition.count(x + y * mCols) > 0) {
            return mEntitiesPosition.at(x + y * mCols).empty();
        }
        else {
            return true;
        }
    }

    bool SetUnit(int x, int y, safe::EntityEngine::EntityID id) {
        if (CheckBounds(x, y)) {
            mEntitiesPosition[x + y * mCols].push_back(id);
            return true;
        }
        return false;
    }

    std::vector<safe::EntityEngine::EntityID> GetEntitiesAt(int x, int y) {
        if (CheckBounds(x, y) && mEntitiesPosition.count(x + y * mCols) > 0) {
            return mEntitiesPosition.at(x + y * mCols);
        }
        return std::vector<safe::EntityEngine::EntityID>();
    }

    void pushNeighbours(TileNode node, std::priority_queue<TileNode>& queue) {
        std::vector< TileNode > neighbours;
        int x, y;

        x = node.mX + 1;
        y = node.mY;
        if (CheckBounds(x, y) /*&& IsEmpty(x, y)*/) {
            queue.push(TileNode(x, y, node.mCost - 1));
        }
        x = node.mX - 1;
        y = node.mY;
        if (CheckBounds(x, y) /*&& IsEmpty(x, y)*/) {
            queue.push(TileNode(x, y, node.mCost - 1));
        }
        x = node.mX;
        y = node.mY + 1;
        if (CheckBounds(x, y) /*&& IsEmpty(x, y)*/) {
            queue.push(TileNode(x, y, node.mCost - 1));
        }
        x = node.mX;
        y = node.mY - 1;
        if (CheckBounds(x, y) /*&& IsEmpty(x, y)*/) {
            queue.push(TileNode(x, y, node.mCost - 1));
        }
    }

    std::vector< TileNode > Dijstra(TileNode init) {
        std::vector < std::vector< bool >> closedNodes(mCols, std::vector<bool>(mRows, false));
        std::priority_queue< TileNode > openNodes;

        std::vector< TileNode > result;

        TileNode current = init;
        closedNodes[init.mX][init.mY] = true;
        pushNeighbours(current, openNodes);
        int iters = 0;
        while (!openNodes.empty() && current.mCost >= 0) {
            current = openNodes.top();
            openNodes.pop();

            if (!closedNodes[current.mX][current.mY]) {
                result.push_back(current);
                if (current.mCost > 0) {
                    pushNeighbours(current, openNodes);
                }
            }
            closedNodes[current.mX][current.mY] = true;
            iters++;
        }
        
        // Remove already occuped tiles
        result.erase(
            std::remove_if(
                result.begin(), result.end(),
                [&](TileNode n){
                    return !IsEmpty(n.mX, n.mY); 
                }),
            result.end());

        return result;
    }

public:
    std::unordered_map< int, std::vector<safe::EntityEngine::EntityID> > mEntitiesPosition;


private:
    safe::Vector3 mOrigin;
    int mCols;
    int mRows;
    int mTileWidth;
    int mTileHeight;

};

#endif // TILEMAP_H
