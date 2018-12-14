local sp = require "sprite_data"
local proc = require "procedural"
local units = require "units"

local entities = {}
--
-- Set space
--
local space = safe.create_space("combat")

--
-- Set systems
--
active_systems = {
  -- Safe systems
  "CameraMovementSystem",
  "RenderSystem",
  "SpriteSheetAnimatorSystem",
  
  -- Cpp systems
  "CharacterGUISystem",
  "DragMovementSystem",
  "GridMovementSystem",
  "TileMapUpdateSystem",
  "TurnOrderSystem",
  
  -- Lua systems
  "DamagePhaseSystem",
  "StatsModifierSystem"
}

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
        DraggableComponent = {
            turn_horizontal = true
        },
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
        DraggableComponent = {
            turn_horizontal = true
        },
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

-- Create entities
for i, e in ipairs(entities) do
    space:create_entity(e)
end

-- Set Systems
safe.set_active_systems( active_systems, true)
