require("lua.animation_data")

local function generate_tile(xpos, ypos, sprite_filename, has_collider)
    local tile = {
        TransformComponent = { 
            position = { x=xpos, y=ypos },
        },
        SpriteComponent = {
            filename = sprite_filename,
            center = { x=0.5, y=1.0 },
            is_vertical = has_collider 
        },
        DraggableComponent = {}
    }
    if has_collider then
        tile['ColliderComponent'] = { 
            shape = 'rectangle', 
            type = 'static', 
            size = {x=12,y=6}, 
            center = {x=0,y=-12}
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

entities = {}

for j, row in pairs(raw_map) do
    for i, col in pairs(row) do
        sprite = 'assets/floor_tile.png'
        has_collider = false
        if col == 1 then
            sprite = 'assets/wall_tile.png'
            has_collider = true
        end

        entities[#entities+1] = generate_tile(i*24-1+6, j*24-1+6, sprite, has_collider)
    end
end

local enemy = {
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
        size = {x=6, y=6},
        center = { x=0, y=-20 }
    },

    DraggableComponent = { is_draggable = true }
}

local player = {
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
        size = {x=6, y=6},
        center = { x=0, y=-20 }
    }
}

--entities[#entities+1] = player
--entities[#entities+1] = enemy
    
entities[#entities+1] = {
    TransformComponent = {
        position = { x=200, y=100 },
    },
    
    SpriteComponent = {
        filename = "./assets/Jackal.png",
        center = { x=0.5, y=1.0 }
    },

    DraggableComponent = {},

    CharacterDataComponent = {
        base_health = 10,
        base_movement = 2
    }
}