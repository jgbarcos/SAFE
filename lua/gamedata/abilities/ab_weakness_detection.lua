local weakness_detection_ability = {
  name =        "weakness_detection",
  category =    "active",
  endturn =     false,
  targetting =  "self",
  
  effects_template = {
  },
  
  modifiers = {
    { name = "weakness_detection",
      stacks = false,
      event = "end_turn",
      event_count = 1,
      stats = {
        attack = { add = 2 }
      }
    }
  }
}

gamedata.abilities_template["weakness_detection"] = weakness_detection_ability