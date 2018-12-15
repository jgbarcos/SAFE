local ability_name = "weakness_detection"
local weakness_detection_ability = {
  name =        ability_name,
  category =    "active",
  endturn =     false,
  targetting =  "self",
  
  effects_template = {
    { name = "dialog", 
      text = "[LUA DEBUG] Ability " .. ability_name .. " was used!" },
  },
  
  modifiers = {
    { name = ability_name,
      stacks = false,
      event = "end_turn",
      event_count = 1,
      stats = {
        attack = { add = 2 }
      }
    }
  }
}

gamedata.abilities.templates[ability_name] = weakness_detection_ability