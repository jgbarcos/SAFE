local cripple_shoot_ability = {
  name =        "cripple_shoot",
  category =    "active",
  endturn =     true,
  targetting =  "relative_tiles",
  
  effects_template = {
    { name = "edit_context",
      edit_context = { damage = 1 }
    },
    { name = "damage_tile" },
    { name = "push_horizontal",
      tiles = 2 },
  },
  
  modifiers = {
    { name = "cripple_shoot",
      stacks = false,
      stats = {
        movement = { add = -1 }
      }
    }
  }
}

gamedata.abilities.templates["cripple_shoot"] = cripple_shoot_ability