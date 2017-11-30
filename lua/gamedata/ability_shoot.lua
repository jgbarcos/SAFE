local Ability = require "game.ability"

local ShootAbility = class("ShootAbility", Ability)
function ShootAbility:initialize(params)
  params =            util.default( params,             {} ) 
  params.name =       util.default( params.name,        "shoot" )
  params.category =   util.default( params.category,    "active" )
  params.targetting = util.default( params.targetting,  "relative_fixed" )
  Ability.initialize(self, params)
  
  self.damage_tile = gamedata.effects["damage_tile"]:new()
  self.effects[#self.effects+1] = self.damage_tile
end

function ShootAbility:perform(context) 
  local ent = safe.get_entity(context.owner)
  local char_data = safe.get_component(ent, "CharacterDataComponent")
  
  context.damage = char_data.base_attack
  
  Ability.perform(self, context)
end

gamedata.abilities_class["shoot"] = ShootAbility