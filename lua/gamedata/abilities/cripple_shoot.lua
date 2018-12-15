local ability_name = "cripple_shoot"
local cripple_shoot_ability = {
  name =        ability_name,
  category =    "active",
  endturn =     true,
  targetting =  "relative_tiles",
  
  effects_template = {
    { name = "edit_context",
      edit_context = { damage = 1 }
    },
    { name = "damage_tile" },
    { name = "push_horizontal",
      tiles = 1 },
    { name = "dialog", 
      text = "[LUA DEBUG] Ability " .. ability_name .. " was used!" },
  },
  
  modifiers = {
    { name = ability_name,
      stacks = false,
      stats = {
        movement = { add = -1 }
      }
    }
  }
}

gamedata.abilities.templates[ability_name] = cripple_shoot_ability