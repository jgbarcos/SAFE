require("lua.animation_data")

local function generate_tile(xpos, ypos, sprite_filename, has_collider)
    local tile = {
        TransformComponent = { position = { x=xpos, y=ypos } },
        SpriteComponent = { filename = sprite_filename, is_vertical = has_collider }
    }

    if has_collider then
        tile['ColliderComponent'] = { 
            shape = 'rectangle', 
            type = 'static', 
            size = {x=12,y=12} 
        }
    end

    return tile
end

local raw_map = {
    {1,1,1,1,1,1,1},
    {1,0,0,1,0,0,1},
    {1,0,0,1,0,0,1},
    {1,0,1,1,0,0,1},
    {1,0,0,0,0,0,1},
    {1,0,0,0,0,0,1},
    {1,1,1,0,1,1,1},
}

map_tiles = {}

count = 1
for j, row in pairs(raw_map) do
    for i, col in pairs(row) do
        sprite = 'assets/floor_tile.png'
        has_collider = false
        if col == 1 then
            sprite = 'assets/wall_tile.png'
            has_collider = true
        end

        map_tiles[count] = generate_tile(i*24-1+6, j*24-1+6, sprite, has_collider)

        count = count + 1
    end
end

enemy = {
    TransformComponent = {
        position = { x=300, y=300 }
    },
    
    SpriteComponent = {
        filename = "./assets/EnemySheet.png",
        center = { x=0.5, y=1.0 }
    },

    SheetAnimationComponent = {
        animations = dir_4_frames_2
    },

    PlayerControlsComponent = {
        up = "UP",
        down = "DOWN",
        left = "LEFT",
        right = "RIGHT"
    },

    ColliderComponent = {
        shape = 'circle',
        size = {x=4, y=4},
        center = { x=0, y=-10 }
    }
}

player = {
    TransformComponent = {
        position = { x=400, y=400 }
    },

    SpriteComponent = {
        filename = "./assets/PlayerSheet.png",
        center = { x=0.5, y=1.0 }
    },

    SheetAnimationComponent = {
        animations = dir_4_frames_3
    },

    PlayerControlsComponent = {
        up = "W",
        down = "S",
        left = "A",
        right = "D"
    },

    ColliderComponent = {
        shape = 'circle',
        size = {x=4, y=4},
        center = { x=0, y=-10 }
    }
}