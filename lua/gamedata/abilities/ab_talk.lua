local talk_ability = {
  name =      "talk",
  category =  "active",
  endturn =   false,
  effects_template = {
    { name = "dialog", text = "Hello there" }
  }
}

gamedata.abilities_template["talk"] = talk_ability