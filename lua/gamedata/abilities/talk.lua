local talk_ability = {
  name =      "talk",
  category =  "active",
  endturn =   false,
  effects_template = {
    { name = "dialog", text = "Hello there" }
  }
}

gamedata.abilities.templates["talk"] = talk_ability