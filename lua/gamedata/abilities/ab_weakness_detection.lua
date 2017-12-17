local weakness_detection_ability = {
  name =        "weakness_detection",
  category =    "active",
  endturn =     false,
  targetting =  "self",
  
  modifiers = {
    { name = "weakness_detection",
      stacks = false,
      stats = {
        attack = { add = 2 }
      }
    }
  }
}

gamedata.abilities_template["weakness_detection"] = weakness_detection_ability