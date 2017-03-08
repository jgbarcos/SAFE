sp = require "lua.sprite_data"

-- This way if the function name changes, only have to change it in one place
local entities = {}
local templates = {}

-- 
-- Create Templates
--
templates[#templates+1] = {
    TemplateName = "Tile",
    TransformComponent = { 
        position = {x=0, y=0, z=-10},
        scale = {x=1, y=1, z=1} 
    },
    GridTileComponent = {},
    SpriteComponent = { 
        filename = 'assets/floor_tile.png',
        center = {x=0.5, y=0.5},
        is_vertical = false
    }
}

templates[#templates+1] = {
    TemplateName = "Cursor",
    SpriteComponent = { 
        filename = "assets/Cursor.png", 
        center = {x=0.5, y=0.5}
    },
    TransformComponent = { 
        position = {x=0, y=0, z=-3},
        scale = {x=1, y=1, z=1} 
    }
}

templates[#templates+1] = {
    TemplateName = "AreaTile",
    SpriteComponent = { 
        filename = "assets/Area.png", 
        center = {x=0.5, y=0.5},
        is_vertical = false
    },
    TransformComponent = { 
        position = {x=0, y=0, z=-5},
        scale = {x=1, y=1, z=1} 
    }
}

--
-- Create Entities
--
local sprites = {"assets/Rebel_Rifle.png", "assets/Rebel_Spear.png",
    "assets/Mercenary_Pistol.png", "assets/Mercenary_Sniper.png",
    "assets/Armored_Heavy.png", "assets/Rebel_Flamefighter.png",
    "assets/Soldier_Assault.png", "assets/Soldier_Shield.png"
}

for i, sprite in ipairs(sprites) do
    entities[#entities+1] = {
        TransformComponent = {
            position = { x=-100, y=-50 },
        scale = {x=1, y=1, z=1} 
        },
        SpriteComponent = {
            filename = sprite,
            center = sp.get(sp.center, sprite)
        },
        DraggableComponent = {},
        GridUnitComponent = { x=2, y=i },
        CharacterDataComponent = {
            base_health = 10,
            base_movement = 2
        }
    }
end

for i, sprite in ipairs(sprites) do
    entities[#entities+1] = {
        TransformComponent = {
            position = { x=100, y=-50 },
        scale = {x=-1, y=1, z=1} 
        },
        SpriteComponent = {
            filename = sprite,
            center = sp.get(sp.center, sprite)
        },
        DraggableComponent = {},
        GridUnitComponent = { x=8, y=i },
        CharacterDataComponent = {
            base_health = 10,
            base_movement = 2
        }
    }
end




--[[
local enemy = {
    TransformComponent = {
        position = { x=300, y=300 },
        scale = { x=2, y=2, z=1 }
    },
    
    SpriteComponent = {
        filename = "./assets/EnemySheet.png",
        center = { x=0.5, y=1.0 }
    },

    SheetAnimationComponent = {
        animations = animations_data["dir_4_frames_2"]
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
        position = { x=400, y=200 },
        scale = { x=2, y=2, z=1 }
    },

    SpriteComponent = {
        filename = "./assets/PlayerSheet.png",
        center = { x=0.5, y=1.0 }
    },

    SheetAnimationComponent = {
        animations = animations_data["dir_4_frames_3"]
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

entities[#entities+1] = player
entities[#entities+1] = enemy

--]]--

--[[
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
]]--

-- Create entities and templates

for i, e in ipairs(entities) do
    safe.create_entity(e)
end
for i, t in ipairs(templates) do
    safe.create_template(t)
end
    