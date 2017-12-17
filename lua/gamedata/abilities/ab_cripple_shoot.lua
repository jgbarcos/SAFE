local cripple_shoot_ability = {
  name =        "cripple_shoot",
  category =    "active",
  endturn =     true,
  targetting =  "relative_tiles",
  
  effects_template = {
    { name = "edit_context",
      edit_context = { damage = 1 }
    },
    { name = "damage_tile" }
  },
  
  modifiers = {
    { name = "cripple_shoot",
      stacks = false,
      stats = {
        movement = { add = -2 }
      }
    }
  }
}

gamedata.abilities_template["cripple_shoot"] = cripple_shoot_ability