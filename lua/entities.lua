local sp = require "sprite_data"
local proc = require "procedural"
local units = require "units"

local entities = {}
--
-- Create Entities
--
local pre = "assets/"
local post = ".png"
local names = {"Rebel_Rifle", "Rebel_Spear",
    "Mercenary_Pistol", "Mercenary_Sniper",
    "Armored_Heavy", "Rebel_Flamefighter",
    "Soldier_Assault", "Soldier_Shield",
    "Soldier_Sniper"
}

local classes = {
  Rebel_Rifle = {"Ranged", "Assault"},
  Rebel_Spear = "Melee",
  Mercenary_Pistol = "Assault",
  Mercenary_Sniper = "Ranged",
  Armored_Heavy = "Heavy",
  Rebel_Flamefighter = {"Melee", "Support"},
  Soldier_Assault = "Assault",
  Soldier_Shield = {"Heavy", "Support"},
  Soldier_Sniper = {"Ranged"},
}

-- Force "Soldier_Assault"
names = {"Soldier_Assault"}

local num_units = 1

for i=1,num_units,1 do
    name = proc:random_element(names)

    local class = proc:random_element(classes[name])
    local unit = units:gen_archetype(class)
    unit.char_data.name = name.."("..class..")"
    
    local ent = {
        TransformComponent = {
            position = { x=-100, y=-50 },
            scale = {x=1, y=1, z=1} 
        },
        SpriteComponent = {
            filename = pre..name..post,
            center = sp.get(sp.center, pre..name..post)
        },
        DraggableComponent = {},
        GridUnitComponent = { x=i, y=0, team=0 },
        CharacterDataComponent = unit.char_data,
        AbilitiesComponent = { ability_names = unit.abilities }
    }

    if sp.sheet[pre..name..post] then
        ent["SheetAnimationComponent"] = {
            animations = sp.sheet[pre..name..post],
            start_anim = "idle"
        }
    end

    entities[#entities+1] = ent
end

for i=1,num_units,1 do
    name = proc:random_element(names)

    local class = proc:random_element(classes[name])
    local unit = units:gen_archetype(class)
    unit.char_data.name = name.."("..class..")"

    local ent = {
        TransformComponent = {
            position = { x=-100, y=-50 },
            scale = {x=-1, y=1, z=1} 
        },
        SpriteComponent = {
            filename = pre..name..post,
            center = sp.get(sp.center, pre..name..post)
        },
        DraggableComponent = {},
        GridUnitComponent = { x=i+num_units+5, y=0, team=1 },
        CharacterDataComponent = unit.char_data,
        AbilitiesComponent = { ability_names = unit.abilities }
    }

    if sp.sheet[pre..name..post] then
        ent["SheetAnimationComponent"] = {
            animations = sp.sheet[pre..name..post],
            start_anim = "idle"
        }
    end

    entities[#entities+1] = ent
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

-- Create entities

for i, e in ipairs(entities) do
    safe.create_entity(e)
end
    
