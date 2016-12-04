#ifndef TILE_H
#define TILE_H

#include "SAFE/Entity.h"

namespace safe {
    
class Tile {
public:
    Tile(Entity* entity)
        : mpEntity(entity) {}
    
    
    Entity* mpEntity;
};

} // namespace safe

#endif /* TILE_H */

